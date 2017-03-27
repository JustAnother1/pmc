/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>
 *
 */

#include <end_stop_handling.h>
#include "com.h"
#include "command_queue.h"
#include "device_heater.h"
#include "device_stepper.h"
#include "device_input.h"
#include "error.h"
#include "events.h"
#include "fw_cfg.h"
#include "hal_buzzer.h"
#include "hal_cfg.h"
#include "hal_cpu.h"
#include "hal_debug.h"
#include "hal_power.h"
#include "hal_pwm.h"
#include "hal_time.h"
#include "protocol.h"
#include "orderhandler.h"
#include "step.h"
#include "uart.h"
#include "usb.h"


#if defined(HAS_USB) && defined(HAS_UART)
#error "Can only have UART or USB not both !"
#endif

#if !defined(HAS_USB) && !defined(HAS_UART)
#error "at least one interface(UART USB) needs to be enabled !"
#endif



#define WRONG_LENGTH      0
#define NUMBER_OF_STRINGS 1

#define NOT_MATCHING_SEQUENCE_NUMBER  255

static uint8_t send_buffer[MAX_SEND_FRAME_SIZE];
#define CS_STOPPED       0
#define CS_CLEARED       1
static uint_fast8_t client_state = CS_STOPPED;
static uint32_t last_host_activity_tick = 0;
static uint32_t host_timeout_tick = 30000; // 30 seconds
static uint_fast8_t recovery_options = RECOVERY_CONDITION_CLEARED;
static uint_fast8_t cause = STOPPED_CAUSE_RESET;
static uint_fast8_t last_sequence_number = NOT_MATCHING_SEQUENCE_NUMBER; // Invalid Number so that it does not match
static uint_fast16_t cached_length;
static uint_fast8_t cur_sequence_number = NOT_MATCHING_SEQUENCE_NUMBER;
static uint_fast8_t string_length[NUMBER_OF_STRINGS] = {15};
static const char *strings[NUMBER_OF_STRINGS] = {"wrong length : "};

static const uint_fast8_t crc_array[256] =
{
       //  0      1      2      3      4      5      6      7      8      9      A      B      C      D      E      F
/* 0*/ 0x00U, 0xa6U, 0xeaU, 0x4cU, 0x72U, 0xd4U, 0x98U, 0x3eU, 0xe4U, 0x42U, 0x0eU, 0xa8U, 0x96U, 0x30U, 0x7cU, 0xdaU,
/* 1*/ 0x6eU, 0xc8U, 0x84U, 0x22U, 0x1cU, 0xbaU, 0xf6U, 0x50U, 0x8aU, 0x2cU, 0x60U, 0xc6U, 0xf8U, 0x5eU, 0x12U, 0xb4U,
/* 2*/ 0xdcU, 0x7aU, 0x36U, 0x90U, 0xaeU, 0x08U, 0x44U, 0xe2U, 0x38U, 0x9eU, 0xd2U, 0x74U, 0x4aU, 0xecU, 0xa0U, 0x06U,
/* 3*/ 0xb2U, 0x14U, 0x58U, 0xfeU, 0xc0U, 0x66U, 0x2aU, 0x8cU, 0x56U, 0xf0U, 0xbcU, 0x1aU, 0x24U, 0x82U, 0xceU, 0x68U,
/* 4*/ 0x1eU, 0xb8U, 0xf4U, 0x52U, 0x6cU, 0xcaU, 0x86U, 0x20U, 0xfaU, 0x5cU, 0x10U, 0xb6U, 0x88U, 0x2eU, 0x62U, 0xc4U,
/* 5*/ 0x70U, 0xd6U, 0x9aU, 0x3cU, 0x02U, 0xa4U, 0xe8U, 0x4eU, 0x94U, 0x32U, 0x7eU, 0xd8U, 0xe6U, 0x40U, 0x0cU, 0xaaU,
/* 6*/ 0xc2U, 0x64U, 0x28U, 0x8eU, 0xb0U, 0x16U, 0x5aU, 0xfcU, 0x26U, 0x80U, 0xccU, 0x6aU, 0x54U, 0xf2U, 0xbeU, 0x18U,
/* 7*/ 0xacU, 0x0aU, 0x46U, 0xe0U, 0xdeU, 0x78U, 0x34U, 0x92U, 0x48U, 0xeeU, 0xa2U, 0x04U, 0x3aU, 0x9cU, 0xd0U, 0x76U,
/* 8*/ 0x3cU, 0x9aU, 0xd6U, 0x70U, 0x4eU, 0xe8U, 0xa4U, 0x02U, 0xd8U, 0x7eU, 0x32U, 0x94U, 0xaaU, 0x0cU, 0x40U, 0xe6U,
/* 9*/ 0x52U, 0xf4U, 0xb8U, 0x1eU, 0x20U, 0x86U, 0xcaU, 0x6cU, 0xb6U, 0x10U, 0x5cU, 0xfaU, 0xc4U, 0x62U, 0x2eU, 0x88U,
/* A*/ 0xe0U, 0x46U, 0x0aU, 0xacU, 0x92U, 0x34U, 0x78U, 0xdeU, 0x04U, 0xa2U, 0xeeU, 0x48U, 0x76U, 0xd0U, 0x9cU, 0x3aU,
/* B*/ 0x8eU, 0x28U, 0x64U, 0xc2U, 0xfcU, 0x5aU, 0x16U, 0xb0U, 0x6aU, 0xccU, 0x80U, 0x26U, 0x18U, 0xbeU, 0xf2U, 0x54U,
/* C*/ 0x22U, 0x84U, 0xc8U, 0x6eU, 0x50U, 0xf6U, 0xbaU, 0x1cU, 0xc6U, 0x60U, 0x2cU, 0x8aU, 0xb4U, 0x12U, 0x5eU, 0xf8U,
/* D*/ 0x4cU, 0xeaU, 0xa6U, 0x00U, 0x3eU, 0x98U, 0xd4U, 0x72U, 0xa8U, 0x0eU, 0x42U, 0xe4U, 0xdaU, 0x7cU, 0x30U, 0x96U,
/* E*/ 0xfeU, 0x58U, 0x14U, 0xb2U, 0x8cU, 0x2aU, 0x66U, 0xc0U, 0x1aU, 0xbcU, 0xf0U, 0x56U, 0x68U, 0xceU, 0x82U, 0x24U,
/* F*/ 0x90U, 0x36U, 0x7aU, 0xdcU, 0xe2U, 0x44U, 0x08U, 0xaeU, 0x74U, 0xd2U, 0x9eU, 0x38U, 0x06U, 0xa0U, 0xecU, 0x4aU
};

static void check_host_timeout(void);
static void handle_resume(uint_fast8_t parameter);
static void send_firmware_id_frame(void);
static void send_unknown_order_response(uint_fast8_t request);
static void send_cached_frame(void);
static void send_stopped_response(void);
static void send_frame(uint_fast16_t length);
static void handle_frame(uint_fast8_t order, uint_fast8_t parameter_length, uint_fast8_t control);
static void fill_in_byte(uint_fast8_t value, uint8_t *position);
static void fill_in_nibble(uint_fast8_t value, uint8_t *position);
static uint_fast8_t crc8(uint_fast8_t length);


void com_init(void)
{
    bool res;
    send_buffer[0] = CLIENT_FRAME_SYNC;
#ifdef HAS_USB
    res = start_usb();
#endif
#ifdef HAS_UART
    res = start_uart();
#endif
    if(false == res)
    {
        error_signal_error_and_die();
    }
    hal_cpu_add_ms_tick_function_cycle(check_host_timeout, 1000);
    send_firmware_id_frame();
}

void com_tick(void)
{
#ifdef HAS_UART
    if(true == uart_has_next_frame())
    {
        handle_frame(uart_get_order(),
                    uart_get_parameter_length(),
                    uart_get_control());
        uart_forget_frame();
    }
#endif
#ifdef HAS_USB
    if(true == usb_has_next_frame())
    {
        handle_frame(usb_get_order(),
                    usb_get_parameter_length(),
                    usb_get_control());
        usb_forget_frame();
    }
#endif
}

uint_fast8_t com_get_parameter_byte(uint_fast8_t index)
{
#ifdef HAS_USB
        return usb_get_parameter_byte(index);
#endif
#ifdef HAS_UART
        return uart_get_parameter_byte(index);
#endif
}

uint_fast8_t com_get_host_timeout_sec(void)
{
    return (uint_fast8_t)(host_timeout_tick/1000);
}

static void check_host_timeout(void)
{
    if(CS_STOPPED == client_state)
    {
        // nothing to do
    }
    else
    {
        uint32_t now = hal_cpu_get_ms_tick();
        if(last_host_activity_tick + host_timeout_tick < now)
        {
            debug_line(STR("Host Timeout !!!"));
            gotoStoppedMode(STOPPED_CAUSE_HOST_TIMEOUT, RECOVERY_CONDITION_CLEARED);
        }
        // else wait a bit longer
    }
}

void gotoStoppedMode(uint_fast8_t cause_for_stopped_mode, uint_fast8_t recovery_options_for_stopped_mode)
{
    uint_fast8_t i;
    client_state = CS_STOPPED;
    cause = cause_for_stopped_mode;
    recovery_options = recovery_options_for_stopped_mode;
    // stop executing the queue
    cmd_queue_clear();
    // power off all heaters
    for(i = 0; i < hal_pwm_get_amount(); i++)
    {
        hal_pwm_set_on_time(i, 0);
    }
    // disable all steppers
    step_disable_all_motors();
    // disable all buzzers
    for(i = 0; i < hal_buzzer_get_amount(); i++)
    {
        hal_buzzer_set_frequency(i, 0);
    }
    // if the board can do it:
    hal_power_off_HighVoltage();
    // Caution: 12V needs to stay on !
    // Fans are on 12V and we do want them to continue cooling the hot end !
}

static void handle_frame(uint_fast8_t order, uint_fast8_t parameter_length, uint_fast8_t control)
{
    if(REQUEST_CONTROL_RESYNC_FLAG_FLAG == (REQUEST_CONTROL_RESYNC_FLAG_FLAG & control))
    {
        // flush cached frame
        last_sequence_number = NOT_MATCHING_SEQUENCE_NUMBER;
    }
    // else do nothing
    cur_sequence_number = 0x0f & control;
    if(cur_sequence_number == last_sequence_number)
    {
        // cached Reply
        send_cached_frame();
        return;
    }
    // else:

    if(CS_STOPPED == client_state)
    {
        switch(order)
        {
        case ORDER_RESUME:
            if(1 == parameter_length)
            {
                handle_resume(com_get_parameter_byte(0));
            }
            else
            {
                com_send_generic_application_error_response_with_comment(GENERIC_ERROR_BAD_PARAMETER_FORMAT,
                                                                         WRONG_LENGTH,
                                                                         parameter_length);
            }
            break;
        default: // Order Code not allowed in Stopped state
            send_stopped_response();
            break;
        }
    }
    else
    {
        last_host_activity_tick = hal_cpu_get_ms_tick();
        switch(order)
        {
        case ORDER_RESUME:
            if(1 == parameter_length)
            {
                handle_resume(com_get_parameter_byte(0));
            }
            else
            {
                com_send_generic_application_error_response_with_comment(GENERIC_ERROR_BAD_PARAMETER_FORMAT,
                                                                         WRONG_LENGTH,
                                                                         parameter_length);
            }
            break;

        case ORDER_REQUEST_INFORMATION:
            if(1 == parameter_length)
            {
                oh_handle_request_information(com_get_parameter_byte(0));
            }
            else
            {
                com_send_generic_application_error_response_with_comment(GENERIC_ERROR_BAD_PARAMETER_FORMAT,
                                                                         WRONG_LENGTH,
                                                                         parameter_length);
            }
            break;

        case ORDER_REQUEST_DEVICE_NAME:
            if(2 == parameter_length)
            {
                oh_handle_request_device_name(com_get_parameter_byte(0), com_get_parameter_byte(1));
            }
            else
            {
                com_send_generic_application_error_response_with_comment(GENERIC_ERROR_BAD_PARAMETER_FORMAT,
                                                                         WRONG_LENGTH,
                                                                         parameter_length);
            }
            break;

        case ORDER_REQUEST_TEMPERATURE_READING:
            if((1 < parameter_length) && (0 == parameter_length%2))
            {
                oh_handle_request_temperature_reading(parameter_length);
            }
            else
            {
                com_send_generic_application_error_response_with_comment(GENERIC_ERROR_BAD_PARAMETER_FORMAT,
                                                                         WRONG_LENGTH,
                                                                         parameter_length);
            }
            break;

        case ORDER_GET_HEATER_CONFIGURATION:
            if(1 == parameter_length)
            {
                dev_heater_get_configuration(com_get_parameter_byte(0));
            }
            else
            {
                com_send_generic_application_error_response_with_comment(GENERIC_ERROR_BAD_PARAMETER_FORMAT,
                                                                         WRONG_LENGTH,
                                                                         parameter_length);
            }
            break;

        case ORDER_CONFIGURE_HEATER:
            if(2 == parameter_length)
            {
                dev_heater_set_temperature_sensor(com_get_parameter_byte(0), com_get_parameter_byte(1));
            }
            else
            {
                com_send_generic_application_error_response_with_comment(GENERIC_ERROR_BAD_PARAMETER_FORMAT,
                                                                         WRONG_LENGTH,
                                                                         parameter_length);
            }
            break;

        case ORDER_SET_HEATER_TARGET_TEMPERATURE:
            if(3 == parameter_length)
            {
                if(true == dev_heater_set_target_temperature(com_get_parameter_byte(0), (com_get_parameter_byte(1) <<8) + com_get_parameter_byte(2)))
                {
                    com_send_ok_response();
                }
                else
                {
                    com_send_generic_application_error_response(GENERIC_ERROR_BAD_PARAMETER_VALUE);
                }
            }
            else
            {
                com_send_generic_application_error_response_with_comment(GENERIC_ERROR_BAD_PARAMETER_FORMAT,
                                                                         WRONG_LENGTH,
                                                                         parameter_length);
            }
            break;

        case ORDER_REQUEST_STATE_OF_SWITCH:
            if((1 < parameter_length) && (0 == parameter_length%2))
            {
                oh_handle_request_state_of_switch(parameter_length);
            }
            else
            {
                com_send_generic_application_error_response_with_comment(GENERIC_ERROR_BAD_PARAMETER_FORMAT,
                                                                         WRONG_LENGTH,
                                                                         parameter_length);
            }
            break;

        case ORDER_SWITCH_AN_OUTPUT:
            if((2 < parameter_length) && (0 == parameter_length%3))
            {
                oh_handle_switch_an_output(parameter_length);
            }
            else
            {
                com_send_generic_application_error_response_with_comment(GENERIC_ERROR_BAD_PARAMETER_FORMAT,
                                                                         WRONG_LENGTH,
                                                                         parameter_length);
            }
            break;

        case ORDER_SET_PWM_OUTPUT_VALUE:
            if(4 == parameter_length)
            {
                oh_handle_set_pwm_output_value(  com_get_parameter_byte(0),
                                                 com_get_parameter_byte(1),
                                                (com_get_parameter_byte(2) <<8)
                                               + com_get_parameter_byte(3) );
            }
            else
            {
                com_send_generic_application_error_response_with_comment(GENERIC_ERROR_BAD_PARAMETER_FORMAT,
                                                                         WRONG_LENGTH,
                                                                         parameter_length);
            }
            break;

        case ORDER_WRITE_FIRMWARE_CONFIGURATION_VALUE:
            if(0 < parameter_length)
            {
                fw_cfg_set_value_to(parameter_length);
            }
            else
            {
                com_send_generic_application_error_response_with_comment(GENERIC_ERROR_BAD_PARAMETER_FORMAT,
                                                                         WRONG_LENGTH,
                                                                         parameter_length);
            }
            break;

        case ORDER_READ_FIRMWARE_CONFIGURATION_VALUE:
            if(0 < parameter_length)
            {
                fw_cfg_get_value_of(parameter_length);
            }
            else
            {
                com_send_generic_application_error_response_with_comment(GENERIC_ERROR_BAD_PARAMETER_FORMAT,
                                                                         WRONG_LENGTH,
                                                                         parameter_length);
            }
            break;

        case ORDER_EMERGENCY_STOP:
            //TODO
            break;

        case ORDER_ACTIVATE_STEPPER_CONTROL:
            if(1 == parameter_length)
            {
                dev_stepper_activate(com_get_parameter_byte(0));
            }
            else
            {
                com_send_generic_application_error_response_with_comment(GENERIC_ERROR_BAD_PARAMETER_FORMAT,
                                                                         WRONG_LENGTH,
                                                                         parameter_length);
            }
            break;

        case ORDER_ENABLE__DIABLE_STEPPER_MOTORS:
            if(2 == parameter_length)
            {
                dev_stepper_enable_motor(com_get_parameter_byte(0), com_get_parameter_byte(1));
            }
            else if(0 == parameter_length)
            {
                dev_stepper_disable_all_motors();
            }
            else
            {
                com_send_generic_application_error_response_with_comment(GENERIC_ERROR_BAD_PARAMETER_FORMAT,
                                                                         WRONG_LENGTH,
                                                                         parameter_length);
            }
            break;

        case ORDER_CONFIGURE_END_STOPS:
            if(3 == parameter_length)
            {
                // one Switch
                if(true == dev_stepper_configure_end_stops(com_get_parameter_byte(0),
                                                com_get_parameter_byte(1),
                                                com_get_parameter_byte(2) ))
                {
                    com_send_ok_response();
                }
                // TODO check else - command already send error
            }
            else if(5 == parameter_length)
            {
                // two switches
                if(true == dev_stepper_configure_end_stops(com_get_parameter_byte(0),
                                                com_get_parameter_byte(1),
                                                com_get_parameter_byte(2) ) )
                {
                    if(true == dev_stepper_configure_end_stops(com_get_parameter_byte(0),
                                                com_get_parameter_byte(3),
                                                com_get_parameter_byte(4) ))
                    {
                        com_send_ok_response();
                    }
                    // TODO check  else - command already send error
                }
                // TODO check  else - command already send error
            }
            else
            {
                com_send_generic_application_error_response_with_comment(GENERIC_ERROR_BAD_PARAMETER_FORMAT,
                                                                         WRONG_LENGTH,
                                                                         parameter_length);
            }
            break;

        case ORDER_ENABLE__DISABLE_END_STOPS:
            if(2 == parameter_length)
            {
                // one Switch
                if(true == dev_input_enable(com_get_parameter_byte(0),
                                 com_get_parameter_byte(1) ))
                {
                    com_send_ok_response();
                }
                // TODO check  else - command already send error
            }
            else if(4 == parameter_length)
            {
                // two switches
                if(true == dev_input_enable(com_get_parameter_byte(0),
                                 com_get_parameter_byte(1) ) )
                {
                    if(true == dev_input_enable(com_get_parameter_byte(2),
                                 com_get_parameter_byte(3) ) )
                    {
                        com_send_ok_response();
                    }
                    // TODO check  else - command already send error
                }
                // TODO check  else - command already send error
            }
            else
            {
                com_send_generic_application_error_response_with_comment(GENERIC_ERROR_BAD_PARAMETER_FORMAT,
                                                                         WRONG_LENGTH,
                                                                         parameter_length);
            }
            break;

        case ORDER_REQUEST_DEVICE_COUNT:
            if(1 == parameter_length)
            {
                oh_handle_request_device_count(com_get_parameter_byte(0));
            }
            else
            {
                com_send_generic_application_error_response_with_comment(GENERIC_ERROR_BAD_PARAMETER_FORMAT,
                                                                         WRONG_LENGTH,
                                                                         parameter_length);
            }
            break;

        case ORDER_QUEUE_COMMAND_BLOCKS:
            if(1 != parameter_length) // 0 bytes is OK to test Queue status but shortest Block is 2 Bytes
            {
                cmd_queue_add_blocks(parameter_length);
            }
            else
            {
                com_send_generic_application_error_response_with_comment(GENERIC_ERROR_BAD_PARAMETER_FORMAT,
                                                                         WRONG_LENGTH,
                                                                         parameter_length);
            }
            break;

        case ORDER_CONFIGURE_AXIS_MOVEMENT_RATES:
            if(5 == parameter_length)
            {
                uint_fast8_t stepper_number;
                uint_fast32_t max_steps;
                stepper_number =             com_get_parameter_byte(0);
                max_steps = (((uint_fast32_t)com_get_parameter_byte(1)) << 24)
                          + (((uint_fast32_t)com_get_parameter_byte(2)) << 16)
                          + (((uint_fast32_t)com_get_parameter_byte(3)) << 8)
                          +  ((uint_fast32_t)com_get_parameter_byte(4));
                dev_stepper_configure_axis_movement_rate(stepper_number, max_steps);
            }
            else
            {
                com_send_generic_application_error_response_with_comment(GENERIC_ERROR_BAD_PARAMETER_FORMAT,
                                                                         WRONG_LENGTH,
                                                                         parameter_length);
            }
            break;

        case ORDER_RETRIEVE_EVENTS:
            if(1 == parameter_length)
            {
                events_receive(com_get_parameter_byte(0));
            }
            else
            {
                com_send_generic_application_error_response_with_comment(GENERIC_ERROR_BAD_PARAMETER_FORMAT,
                                                                         WRONG_LENGTH,
                                                                         parameter_length);
            }
            break;

        case ORDER_GET_NUMBER_OF_EVENT_FORMAT_IDS:
            if(0 == parameter_length)
            {
                events_get_number_format_ids();
            }
            else
            {
                com_send_generic_application_error_response_with_comment(GENERIC_ERROR_BAD_PARAMETER_FORMAT,
                                                                         WRONG_LENGTH,
                                                                         parameter_length);
            }
            break;

        case ORDER_GET_EVENT_STRING_FORMAT_ID:
            if(2 == parameter_length)
            {
                events_get_string_format((((uint_fast16_t)com_get_parameter_byte(0)) << 8) + com_get_parameter_byte(1));
            }
            else
            {
                com_send_generic_application_error_response_with_comment(GENERIC_ERROR_BAD_PARAMETER_FORMAT,
                                                                         WRONG_LENGTH,
                                                                         parameter_length);
            }
            break;

        case ORDER_CLEAR_COMMAND_BLOCK_QUEUE:
            if(0 == parameter_length)
            {
                cmd_queue_clear();
            }
            else
            {
                com_send_generic_application_error_response_with_comment(GENERIC_ERROR_BAD_PARAMETER_FORMAT,
                                                                         WRONG_LENGTH,
                                                                         parameter_length);
            }
            break;

        case ORDER_REQUEST_DEVICE_STATUS:
            if(2 == parameter_length)
            {
                oh_handle_request_device_status(com_get_parameter_byte(0), com_get_parameter_byte(1));
            }
            else
            {
                com_send_generic_application_error_response_with_comment(GENERIC_ERROR_BAD_PARAMETER_FORMAT,
                                                                         WRONG_LENGTH,
                                                                         parameter_length);
            }
            break;

        case ORDER_TRAVERSE_FIMRWARE_CONFIGURATION_VALUES:
            fw_cfg_get_next_value(parameter_length);
            break;

        case ORDER_CONFIGURE_MOVEMENT_UNDERRUN_AVOIDANCE_PARAMETERS:
            if(9 == parameter_length)
            {
                uint_fast8_t stepper_number;
                uint_fast32_t max_steps;
                uint_fast32_t max_accel;
                stepper_number =             com_get_parameter_byte(0);
                max_steps = (((uint_fast32_t)com_get_parameter_byte(1)) << 24)
                          + (((uint_fast32_t)com_get_parameter_byte(2)) << 16)
                          + (((uint_fast32_t)com_get_parameter_byte(3)) << 8)
                          +  ((uint_fast32_t)com_get_parameter_byte(4));
                max_accel = (((uint_fast32_t)com_get_parameter_byte(5)) << 24)
                          + (((uint_fast32_t)com_get_parameter_byte(6)) << 16)
                          + (((uint_fast32_t)com_get_parameter_byte(7)) << 8)
                          +  ((uint_fast32_t)com_get_parameter_byte(8));
                dev_stepper_configure_mvmnt_unrun_avoid_para(stepper_number, max_steps, max_accel);
            }
            else
            {
                com_send_generic_application_error_response_with_comment(GENERIC_ERROR_BAD_PARAMETER_FORMAT,
                                                                         WRONG_LENGTH,
                                                                         parameter_length);
            }
            break;

        case ORDER_GET_FIRMWARE_CONFIGURATION_VALUE_PROPERTIES:
            fw_cfg_get_value_property(parameter_length);
            break;

        case ORDER_SET_OUTPUT_TONE:
            if(4 == parameter_length)
            {
                oh_handle_set_output_tone(com_get_parameter_byte(0), com_get_parameter_byte(1), (com_get_parameter_byte(2) <<8) + com_get_parameter_byte(3));
            }
            else
            {
                com_send_generic_application_error_response_with_comment(GENERIC_ERROR_BAD_PARAMETER_FORMAT,
                                                                         WRONG_LENGTH,
                                                                         parameter_length);
            }
            break;

        case ORDER_RESET:
            com_send_ok_response(); // it might get through
            hal_time_ms_sleep(20); // give the Response a realistic chance
            hal_cpu_do_software_reset(RESET_REASON_HOST_ORDER);
            break;

        // new orders go here...
        default: // invalid Order Code
            send_unknown_order_response(order);
            break;
        }
    }
}

static void handle_resume(uint_fast8_t parameter)
{
    switch(parameter)
    {
    case 0: // Query
        if(CS_CLEARED == client_state)
        {
            // not in stopped state -> send OK
            com_send_ok_response();
        }
        else
        {
            send_stopped_response();
        }
        break;

    case 1: // clear
        // if the board can do it:
        hal_power_on_5V();
        hal_power_on_12V();
        hal_power_on_HighVoltage();
        client_state = CS_CLEARED;
        last_host_activity_tick = hal_cpu_get_ms_tick();
        cmd_queue_reset_executed_commands();
        com_send_ok_response();
        break;

    default:
        com_send_generic_application_error_response(GENERIC_ERROR_BAD_PARAMETER_VALUE);
        break;
    }
}

static uint_fast8_t crc8(uint_fast8_t length)
{
    uint_fast8_t crc = 0;
    uint_fast8_t i = 1;
    while (length-- > 0)
    {
        crc = crc_array[send_buffer[i] ^ crc];
        i++;
    }
    return crc;
}

uint_fast8_t com_crc_a_byte(uint_fast8_t new_byte, uint_fast8_t old_crc)
{
    return crc_array[new_byte ^ old_crc];
}

uint8_t *com_get_start_parameter(void)
{
    return &send_buffer[REPLY_FRAME_START_OF_PARAMETER];
}

uint_fast8_t com_get_max_parameter_length(void)
{
    return MAX_SEND_FRAME_SIZE - 5; // Sync + Length + Control + ReplyCode + CRC
}

static void send_frame(uint_fast16_t length)
{
    if(true == events_has_events())
    {
        send_buffer[REPLY_FRAME_POS_OF_CONTROL] = send_buffer[REPLY_FRAME_POS_OF_CONTROL] | 0x10;
    }
    // else bit 4 is 0
#ifdef HAS_UART
    uart_send_frame(&send_buffer[0], length);
#endif
#ifdef HAS_USB
    usb_send_frame(&send_buffer[0], length);
#endif
}

void com_send_stepper_error_event(uint_fast8_t cur_errors)
{
    send_buffer[REPLY_FRAME_POS_OF_LENGTH] = 4;
    send_buffer[REPLY_FRAME_POS_OF_CONTROL] = cur_sequence_number | REPLY_CONTROL_FLAG_DEBUG;
    send_buffer[REPLY_FRAME_POS_OF_REPLY_CODE] = REPLY_CODE_OK;
    send_buffer[REPLY_FRAME_START_OF_PARAMETER] = 0xbb; // TODO
    send_buffer[REPLY_FRAME_START_OF_PARAMETER + 1] = cur_errors;
    send_buffer[6] = crc8(5);
    send_frame(7);
    // TODO also file with event extension
}

static void send_firmware_id_frame(void)
{
    send_buffer[REPLY_FRAME_POS_OF_LENGTH] = 5;
    send_buffer[REPLY_FRAME_POS_OF_CONTROL] = REPLY_CONTROL_FLAG_DEBUG;
    send_buffer[REPLY_FRAME_POS_OF_REPLY_CODE] = REPLY_CODE_DEBUG;
    send_buffer[REPLY_FRAME_START_OF_PARAMETER] = FIRMWARE_REVISION_MAJOR;
    send_buffer[REPLY_FRAME_START_OF_PARAMETER + 1] = FIRMWARE_REVISION_MINOR;
    send_buffer[REPLY_FRAME_START_OF_PARAMETER + 2] = hal_cpu_get_state_byte();
    send_buffer[7] = crc8(6);
    send_frame(8);
}

void com_send_bad_crc(uint_fast8_t received, uint_fast8_t expected)
{
    send_buffer[REPLY_FRAME_POS_OF_LENGTH] = 5;
    send_buffer[REPLY_FRAME_POS_OF_CONTROL] = cur_sequence_number;
    send_buffer[REPLY_FRAME_POS_OF_REPLY_CODE] = REPLY_CODE_FRAME_RECEIPT_ERROR;
    send_buffer[REPLY_FRAME_START_OF_PARAMETER] = FRAME_ERROR_BAD_CRC;
    send_buffer[REPLY_FRAME_START_OF_PARAMETER + 1] = received;
    send_buffer[REPLY_FRAME_START_OF_PARAMETER + 2] = expected;
    send_buffer[7] = crc8(6);
    send_frame(8);
}

static void send_stopped_response(void)
{
    send_buffer[REPLY_FRAME_POS_OF_LENGTH] = 4;
    send_buffer[REPLY_FRAME_POS_OF_CONTROL] = cur_sequence_number;
    send_buffer[REPLY_FRAME_POS_OF_REPLY_CODE] = REPLY_CODE_STOPPED;
    send_buffer[REPLY_FRAME_START_OF_PARAMETER] = recovery_options;
    send_buffer[REPLY_FRAME_START_OF_PARAMETER + 1] = cause;
    send_buffer[6] = crc8(5);
    send_frame(7);
}

static void send_cached_frame(void)
{
    send_frame(cached_length);
}

void com_send_ok_response(void)
{
    send_buffer[REPLY_FRAME_POS_OF_LENGTH] = 2;
    send_buffer[REPLY_FRAME_POS_OF_CONTROL] = cur_sequence_number;
    send_buffer[REPLY_FRAME_POS_OF_REPLY_CODE] = REPLY_CODE_OK;
    send_buffer[4] = crc8(3);
    send_frame(5);
}

void com_send_ok_response_with_single_byte_parameter(uint_fast8_t parameter)
{
    send_buffer[REPLY_FRAME_POS_OF_LENGTH] = 3;
    send_buffer[REPLY_FRAME_POS_OF_CONTROL] = cur_sequence_number;
    send_buffer[REPLY_FRAME_POS_OF_REPLY_CODE] = REPLY_CODE_OK;
    send_buffer[REPLY_FRAME_START_OF_PARAMETER] = parameter;
    send_buffer[5] = crc8(4);
    send_frame(6);
}

void com_send_ok_response_with_two_byte_parameter(uint_fast8_t parameter1, uint_fast8_t parameter2)
{
    send_buffer[REPLY_FRAME_POS_OF_LENGTH] = 3;
    send_buffer[REPLY_FRAME_POS_OF_CONTROL] = cur_sequence_number;
    send_buffer[REPLY_FRAME_POS_OF_REPLY_CODE] = REPLY_CODE_OK;
    send_buffer[REPLY_FRAME_START_OF_PARAMETER] = parameter1;
    send_buffer[REPLY_FRAME_START_OF_PARAMETER + 1] = parameter2;
    send_buffer[6] = crc8(5);
    send_frame(7);
}

void com_send_order_specific_error_with_prefilled_parameter(uint_fast8_t parameter_length)
{
    send_buffer[REPLY_FRAME_POS_OF_LENGTH] = 2 + parameter_length;
    send_buffer[REPLY_FRAME_POS_OF_CONTROL] = cur_sequence_number;
    send_buffer[REPLY_FRAME_POS_OF_REPLY_CODE] = REPLY_CODE_ORDER_SPECIFIC_ERROR;
    // parameter is already filled in
    send_buffer[4 + parameter_length] = crc8(3 + parameter_length);
    send_frame(5 + parameter_length);
}

void com_send_ok_with_prefilled_parameter(uint_fast8_t parameter_length)
{
    send_buffer[REPLY_FRAME_POS_OF_LENGTH] = 2 + parameter_length;
    send_buffer[REPLY_FRAME_POS_OF_CONTROL] = cur_sequence_number;
    send_buffer[REPLY_FRAME_POS_OF_REPLY_CODE] = REPLY_CODE_OK;
    // parameter is already filled in
    send_buffer[4 + parameter_length] = crc8(3 + parameter_length);
    send_frame(5 + parameter_length);
}

void com_send_debug_frame_with_filled_parameter(uint_fast8_t parameter_length)
{
    send_buffer[REPLY_FRAME_POS_OF_LENGTH] = 2 + parameter_length;
    send_buffer[REPLY_FRAME_POS_OF_CONTROL] = cur_sequence_number | REPLY_CONTROL_FLAG_DEBUG;
    send_buffer[REPLY_FRAME_POS_OF_REPLY_CODE] = REPLY_CODE_OK;
    // parameter is already filled in
    send_buffer[4 + parameter_length] = crc8(3 + parameter_length);
    send_frame(5 + parameter_length);
}

static void fill_in_nibble(uint_fast8_t value, uint8_t *position)
{
    switch(value)
    {
    case 0:  *position = '0'; return;
    case 1:  *position = '1'; return;
    case 2:  *position = '2'; return;
    case 3:  *position = '3'; return;
    case 4:  *position = '4'; return;
    case 5:  *position = '5'; return;
    case 6:  *position = '6'; return;
    case 7:  *position = '7'; return;
    case 8:  *position = '8'; return;
    case 9:  *position = '9'; return;
    case 10: *position = 'A'; return;
    case 11: *position = 'B'; return;
    case 12: *position = 'C'; return;
    case 13: *position = 'D'; return;
    case 14: *position = 'E'; return;
    case 15: *position = 'F'; return;
    }
}

static void fill_in_byte(uint_fast8_t value, uint8_t *position)
{
    uint_fast8_t highNibble = (0xf0 & value) >>4;
    fill_in_nibble(highNibble, position);
    value = value & 0x0f;
    fill_in_nibble(value, position + 1);
}

void com_send_generic_application_error_response_with_comment(uint_fast8_t cause, uint_fast8_t message_index, uint_fast8_t value)
{
    uint_fast8_t i;
    const char *p = strings[message_index];
    send_buffer[REPLY_FRAME_POS_OF_LENGTH] = 5 + string_length[message_index];
    send_buffer[REPLY_FRAME_POS_OF_CONTROL] = cur_sequence_number;
    send_buffer[REPLY_FRAME_POS_OF_REPLY_CODE] = REPLY_CODE_GENERIC_APPLICATION_ERROR;
    send_buffer[REPLY_FRAME_START_OF_PARAMETER] = cause;
    for(i = 0; i < string_length[message_index]; i++)
    {
        send_buffer[REPLY_FRAME_START_OF_PARAMETER + 1 + i] = *p;
        p++;
    }
    fill_in_byte(value, &send_buffer[REPLY_FRAME_START_OF_PARAMETER + 1 + string_length[message_index]]); // this writes 2 bytes
    send_buffer[REPLY_FRAME_START_OF_PARAMETER + 3 + string_length[message_index]]
               = crc8(REPLY_FRAME_START_OF_PARAMETER + 2 + string_length[message_index] );
    send_frame(REPLY_FRAME_START_OF_PARAMETER + 4 + string_length[message_index]);
}

static void send_unknown_order_response(uint_fast8_t the_unknown_order)
{
    send_buffer[REPLY_FRAME_POS_OF_LENGTH] = 4;
    send_buffer[REPLY_FRAME_POS_OF_CONTROL] = cur_sequence_number;
    send_buffer[REPLY_FRAME_POS_OF_REPLY_CODE] = REPLY_CODE_GENERIC_APPLICATION_ERROR;
    send_buffer[REPLY_FRAME_START_OF_PARAMETER] = GENERIC_ERROR_UNKNOWN_ORDER;
    send_buffer[REPLY_FRAME_START_OF_PARAMETER + 1] = the_unknown_order;
    send_buffer[6] = crc8(5);
    send_frame(7);
}

void com_send_generic_application_error_response(uint_fast8_t type)
{
    send_buffer[REPLY_FRAME_POS_OF_LENGTH] = 3;
    send_buffer[REPLY_FRAME_POS_OF_CONTROL] = cur_sequence_number;
    send_buffer[REPLY_FRAME_POS_OF_REPLY_CODE] = REPLY_CODE_GENERIC_APPLICATION_ERROR;
    send_buffer[REPLY_FRAME_START_OF_PARAMETER] = type;
    send_buffer[5] = crc8(4);
    send_frame(6);
}

// end of File
