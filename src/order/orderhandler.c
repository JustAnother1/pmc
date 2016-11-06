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

#include "com.h"
#include "orderhandler.h"
#include "protocol.h"
#include "device_buzzer.h"
#include "device_heater.h"
#include "device_input.h"
#include "device_output.h"
#include "device_pwm.h"
#include "device_stepper.h"
#include "device_temperature_sensor.h"
#include "fw_cfg.h"

static char firmware_name[]= "Pacemaker Client";
// TODO conditional compile for next Hardware / or Hardware auto detect
static char hardware_name[]= "Pacemaker Client";

void oh_handle_request_information(uint_fast8_t parameter)
{
    uint_fast8_t i;
    switch(parameter)
    {
    case 0: // firmware name string
        i = com_copy_string_to_parameter((char*)&firmware_name, com_get_start_parameter());
        com_send_ok_with_prefilled_parameter(i);
        break;

    case 1: // serial number string
        i = fw_cfg_read_setting_to(SETTING_SERIAL_NUMBER, com_get_start_parameter());
        com_send_ok_with_prefilled_parameter(i);
        break;

    case 2: // hardware name string
        i = com_copy_string_to_parameter((char*)&hardware_name, com_get_start_parameter());
        com_send_ok_with_prefilled_parameter(i);
        break;

    case 3: // given name / identity string
        i = fw_cfg_read_setting_to(SETTING_GIVEN_NAME, com_get_start_parameter());
        com_send_ok_with_prefilled_parameter(i);
        break;

    case 4: // major Protocol Version
        com_send_ok_response_with_single_byte_parameter(PROTOCOL_VERSION_MAJOR);
        break;

    case 5: // minor protocol version
        com_send_ok_response_with_single_byte_parameter(PROTOCOL_VERSION_MINOR);
        break;

    case 6: // list of supported protocol extensions
    {
        uint8_t *para_start = com_get_start_parameter();
        para_start[0] = 0; // Stepper Control
        para_start[1] = 1; // Queued Command
        para_start[2] = 2; // Basic Move
        para_start[3] = 3; // Event Reporting
        com_send_ok_with_prefilled_parameter(4);
    }
        break;

    case 7: // firmware type
        com_send_ok_response_with_single_byte_parameter(FIRMWARE_TYPE);
        break;

    case 8: // major Firmware revision
        com_send_ok_response_with_single_byte_parameter(FIRMWARE_REVISION_MAJOR);
        break;

    case 9: // minor firmware revision
        com_send_ok_response_with_single_byte_parameter(FIRMWARE_REVISION_MINOR);
        break;

    case 10: // hardware type
        com_send_ok_response_with_single_byte_parameter(HARDWARE_TYPE);
        break;

    case 11: // hardware revision
        com_send_ok_response_with_single_byte_parameter(1);
        // right now there is only one revision.
        break;

    case 12: // maximum supported Step rate
    {
        uint8_t *para_start = com_get_start_parameter();
        para_start[0] = (uint8_t)(MAX_STEP_RATE >> 24) & 0xff;
        para_start[1] = (uint8_t)(MAX_STEP_RATE >> 16) & 0xff;
        para_start[2] = (uint8_t)(MAX_STEP_RATE >> 8)  & 0xff;
        para_start[3] = (uint8_t) MAX_STEP_RATE        & 0xff;
        com_send_ok_with_prefilled_parameter(4);
    }
        break;

    case 13: // host timeout
        com_send_ok_response_with_single_byte_parameter(30); //TODO
        break;

    default:
        com_send_generic_application_error_response(GENERIC_ERROR_BAD_PARAMETER_VALUE);
        break;
    }
}

void oh_handle_request_device_name(uint_fast8_t device_type, uint_fast8_t device_number)
{
    switch(device_type)
    {
    case DEVICE_TYPE_SWITCH_INPUT:
        com_send_ok_with_prefilled_parameter(dev_input_get_name(device_number, com_get_start_parameter()));
        break;

    case DEVICE_TYPE_SWITCH_OUTPUT:
        com_send_ok_with_prefilled_parameter(dev_output_get_name(device_number, com_get_start_parameter()));
        break;

    case DEVICE_TYPE_PWM_OUTPUT:
        com_send_ok_with_prefilled_parameter(dev_pwm_get_name(device_number, com_get_start_parameter()));
        break;

    case DEVICE_TYPE_STEPPER:
        com_send_ok_with_prefilled_parameter(dev_stepper_get_name(device_number, com_get_start_parameter()));
        break;

    case DEVICE_TYPE_HEATER:
        com_send_ok_with_prefilled_parameter(dev_heater_get_name(device_number, com_get_start_parameter()));
        break;

    case DEVICE_TYPE_TEMPERATURE_SENSOR:
        com_send_ok_with_prefilled_parameter(dev_temperature_sensor_get_name(device_number, com_get_start_parameter()));
        break;

    case DEVICE_TYPE_BUZZER:
        com_send_ok_with_prefilled_parameter(dev_buzzer_get_name(device_number, com_get_start_parameter()));
        break;

    case DEVICE_TYPE_INVALID:
    default:
        com_send_generic_application_error_response(GENERIC_ERROR_BAD_PARAMETER_VALUE);
        break;
    }
}

void oh_handle_request_device_count(uint_fast8_t device_type)
{
    switch(device_type)
    {
    case DEVICE_TYPE_SWITCH_INPUT:
        com_send_ok_response_with_single_byte_parameter(dev_input_get_count());
        break;

    case DEVICE_TYPE_SWITCH_OUTPUT:
        com_send_ok_response_with_single_byte_parameter(dev_output_get_count());
        break;

    case DEVICE_TYPE_PWM_OUTPUT:
        com_send_ok_response_with_single_byte_parameter(dev_pwm_get_count());
        break;

    case DEVICE_TYPE_STEPPER:
        com_send_ok_response_with_single_byte_parameter(dev_stepper_get_count());
        break;

    case DEVICE_TYPE_HEATER:
        com_send_ok_response_with_single_byte_parameter(dev_heater_get_count());
        break;

    case DEVICE_TYPE_TEMPERATURE_SENSOR:
        com_send_ok_response_with_single_byte_parameter(dev_temperature_sensor_get_count());
        break;

    case DEVICE_TYPE_BUZZER:
        com_send_ok_response_with_single_byte_parameter(dev_buzzer_get_count());
        break;

    case DEVICE_TYPE_INVALID:
    default:
        com_send_generic_application_error_response(GENERIC_ERROR_BAD_PARAMETER_VALUE);
        break;
    }
}

void oh_handle_request_device_status(uint_fast8_t device_type, uint_fast8_t device_number)
{
    switch(device_type)
    {
    case DEVICE_TYPE_SWITCH_INPUT:
        com_send_ok_response_with_single_byte_parameter(dev_input_get_status(device_number));
        break;

    case DEVICE_TYPE_SWITCH_OUTPUT:
        com_send_ok_response_with_single_byte_parameter(dev_output_get_status(device_number));
        break;

    case DEVICE_TYPE_PWM_OUTPUT:
        com_send_ok_response_with_single_byte_parameter(dev_pwm_get_status(device_number));
        break;

    case DEVICE_TYPE_STEPPER:
        com_send_ok_response_with_single_byte_parameter(dev_stepper_get_status(device_number));
        break;

    case DEVICE_TYPE_HEATER:
        com_send_ok_response_with_single_byte_parameter(dev_heater_get_status(device_number));
        break;

    case DEVICE_TYPE_TEMPERATURE_SENSOR:
        com_send_ok_response_with_single_byte_parameter(dev_temperature_sensor_get_status(device_number));
        break;

    case DEVICE_TYPE_BUZZER:
        com_send_ok_response_with_single_byte_parameter(dev_buzzer_get_status(device_number));
        break;

    case DEVICE_TYPE_INVALID:
    default:
        com_send_generic_application_error_response(GENERIC_ERROR_BAD_PARAMETER_VALUE);
        break;
    }
}

void oh_handle_set_pwm_output_value(uint_fast8_t device_type, uint_fast8_t device_number, uint_fast16_t pwm)
{
    switch(device_type)
    {
    case DEVICE_TYPE_PWM_OUTPUT:
        dev_pwm_set_pwm(device_number, pwm);
        break;

    case DEVICE_TYPE_BUZZER:
        dev_buzzer_set_pwm(device_number, pwm);
        break;

    case DEVICE_TYPE_HEATER:
    case DEVICE_TYPE_SWITCH_INPUT:
    case DEVICE_TYPE_SWITCH_OUTPUT:
    case DEVICE_TYPE_STEPPER:
    case DEVICE_TYPE_TEMPERATURE_SENSOR:
        com_send_generic_application_error_response(GENERIC_ERROR_INVALID_DEVICE_TYPE);
        break;

    case DEVICE_TYPE_INVALID:
    default:
        com_send_generic_application_error_response(GENERIC_ERROR_BAD_PARAMETER_VALUE);
        break;
    }
}

void oh_handle_set_output_tone(uint_fast8_t device_type, uint_fast8_t device_number, uint_fast16_t frequency)
{
    switch(device_type)
    {
    case DEVICE_TYPE_PWM_OUTPUT:
        dev_pwm_set_frequency(device_number, frequency);
        break;

    case DEVICE_TYPE_BUZZER:
        dev_buzzer_set_frequency(device_number, frequency);
        break;

    case DEVICE_TYPE_HEATER:
    case DEVICE_TYPE_SWITCH_INPUT:
    case DEVICE_TYPE_SWITCH_OUTPUT:
    case DEVICE_TYPE_STEPPER:
    case DEVICE_TYPE_TEMPERATURE_SENSOR:
        com_send_generic_application_error_response(GENERIC_ERROR_INVALID_DEVICE_TYPE);
        break;

    case DEVICE_TYPE_INVALID:
    default:
        com_send_generic_application_error_response(GENERIC_ERROR_BAD_PARAMETER_VALUE);
        break;
    }
}

void oh_handle_request_temperature_reading(uint_fast8_t parameter_length)
{
    uint_fast8_t bytes_done = 0;
    uint_fast16_t temperature = 0;
    uint8_t * result_position = com_get_start_parameter();
    while(1 < parameter_length - bytes_done)
    {
        switch(com_get_parameter_byte(bytes_done + 0))
        {
        case DEVICE_TYPE_HEATER:
            temperature = dev_heater_get_temperature(com_get_parameter_byte(bytes_done + 1));
            break;

        case DEVICE_TYPE_TEMPERATURE_SENSOR:
            temperature = dev_temperature_sensor_get_temperature(com_get_parameter_byte(bytes_done + 1));
            break;

        case DEVICE_TYPE_PWM_OUTPUT:
        case DEVICE_TYPE_BUZZER:
        case DEVICE_TYPE_SWITCH_INPUT:
        case DEVICE_TYPE_SWITCH_OUTPUT:
        case DEVICE_TYPE_STEPPER:
            com_send_generic_application_error_response(GENERIC_ERROR_INVALID_DEVICE_TYPE);
            return;

        case DEVICE_TYPE_INVALID:
        default:
            com_send_generic_application_error_response(GENERIC_ERROR_BAD_PARAMETER_VALUE);
            return;
        }
        *result_position =(uint8_t)(temperature >> 8);
        result_position++;
        *result_position = (uint8_t)(0xff & temperature);
        result_position++;
        bytes_done = bytes_done + 2;
    }
    com_send_ok_with_prefilled_parameter(parameter_length);
}

void oh_handle_request_state_of_switch(uint_fast8_t parameter_length)
{
    uint_fast8_t bytes_done = 0;
    uint_fast8_t switch_state = 0;
    uint8_t * result_position = com_get_start_parameter();
    while(1 < parameter_length - bytes_done)
    {
        switch(com_get_parameter_byte(bytes_done + 0))
        {
        case DEVICE_TYPE_SWITCH_INPUT:
            switch_state = dev_input_get_switch_state(com_get_parameter_byte(bytes_done + 1));
            break;

        case DEVICE_TYPE_SWITCH_OUTPUT:
            switch_state = dev_output_get_switch_state(com_get_parameter_byte(bytes_done + 1));
            break;

        case DEVICE_TYPE_HEATER:
        case DEVICE_TYPE_TEMPERATURE_SENSOR:
        case DEVICE_TYPE_PWM_OUTPUT:
        case DEVICE_TYPE_BUZZER:
        case DEVICE_TYPE_STEPPER:
            com_send_generic_application_error_response(GENERIC_ERROR_INVALID_DEVICE_TYPE);
            return;

        case DEVICE_TYPE_INVALID:
        default:
            com_send_generic_application_error_response(GENERIC_ERROR_BAD_PARAMETER_VALUE);
            return;
        }
        *result_position =(uint8_t)switch_state;
        result_position++;
        bytes_done = bytes_done + 2;
    }
    com_send_ok_with_prefilled_parameter(parameter_length);
}

void oh_handle_switch_an_output(uint_fast8_t parameter_length)
{
    uint_fast8_t bytes_done = 0;
    while(2 < parameter_length - bytes_done)
    {
        switch(com_get_parameter_byte(bytes_done + 0))
        {

        case DEVICE_TYPE_SWITCH_OUTPUT:
            dev_output_set_switch_state(com_get_parameter_byte(bytes_done + 1),
                                        com_get_parameter_byte(bytes_done + 2));
            break;

        case DEVICE_TYPE_SWITCH_INPUT:
        case DEVICE_TYPE_HEATER:
        case DEVICE_TYPE_TEMPERATURE_SENSOR:
        case DEVICE_TYPE_PWM_OUTPUT:
        case DEVICE_TYPE_BUZZER:
        case DEVICE_TYPE_STEPPER:
            com_send_generic_application_error_response(GENERIC_ERROR_INVALID_DEVICE_TYPE);
            return;

        case DEVICE_TYPE_INVALID:
        default:
            com_send_generic_application_error_response(GENERIC_ERROR_BAD_PARAMETER_VALUE);
            return;
        }
        bytes_done = bytes_done + 3;
    }
    com_send_ok_response();
}

// end of File
