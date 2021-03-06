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

#include <stdlib.h>
#include "command_queue.h"
#include "step.h"
#include "protocol.h"
#include "com.h"
#include "device_heater.h"
#include "device_output.h"
#include "orderhandler.h"
#include "device_stepper.h"
#include "device_input.h"
#include "hal_cfg.h"
#include "hal_debug.h"
#include "end_stop_handling.h"

#define MAX_QUEUE_ELEMENTS                50
// Block Envelope is Length and Type -> 2 Bytes
#define BLOCK_ENVELOPE_SIZE               2
// biggest block is a Basic Linear Move with 14 Axis, 2 Bytes for Steps, 2 Bytes Axis selection -> 40 Bytes
// ??? Split those big orders up into more than one slot?
#define MAX_BLOCK_LENGTH                  41

// Positions in Blocks:
#define POS_OF_LENGTH                     0
// command wrapper:
#define POS_OF_ORDER                      1
#define POS_OF_PARAMETER_START            2
// Delay
#define POS_OF_DELAY_MSB                  1
#define POS_OF_DELAY_LSB                  2
// Basic Linear Move
#define POSITION_OF_AXIS_BYTE_MODE_SWITCH 1
#define AXIS_BYTE_MODE_SWITCH_MASK        0x80

static void finished_current_queued_block(void);
static void send_queue_ok_response(void);
static void send_queue_failed_response(uint_fast8_t cause,
                                       uint_fast8_t enqueued_commands,
                                       uint_fast8_t error_reply_code,
                                       char* error_reason);
static void handle_wrapped_command(void);

static uint_fast8_t queue[MAX_QUEUE_ELEMENTS][MAX_BLOCK_LENGTH];
static uint_fast8_t queue_type[MAX_QUEUE_ELEMENTS];
// Index to Queue for:
static volatile uint_fast8_t read_pos; // reading
static volatile uint_fast8_t write_pos; // writing
static uint_fast16_t finished_blocks = 0; // number of successfully executed Blocks

#define STEPPER_MODULE_TEST

#ifdef STEPPER_MODULE_TEST

uint_fast8_t move_data[9] = {
       0, /* 0 : Don't care*/
       1, /* 1: Axis selection field*/
       0, /* 2: num bytes per Step, directions on axis*/
       0, /* 3: primary axis, homing */
       0, /* 4: nominal speed - dynamic */
       0, /* 5: end speed - dynamic change */
    0xff, /* 6: Accelleration steps - becomes 0 when full speed is reached*/
       0, /* 7: decelleration steps - who breaks losses ;-) */
    0xff  /* 8: steps on axis 0 */
};

static uint_fast8_t start_speed = 0;
static uint_fast8_t speed_increment = 1;
static uint_fast8_t max_speed = 12;
static bool enabled = false;

#endif

void cmd_queue_init(void)
{
    cmd_queue_clear();
}

void cmd_queue_reset_executed_commands(void)
{
    finished_blocks = 0;
}

void cmd_queue_clear(void)
{
    read_pos = 0;
    write_pos = 0;
}

#ifdef DEBUG_ACTIVE

bool cmd_queue_show_status(uint8_t* setting)
{
    (void) setting;
    debug_line(STR("Command Queue Status:"));
    debug_line(STR("read position : %d"), read_pos);
    debug_line(STR("write position : %d"), write_pos);
    debug_line(STR("finished blocks : %d"), finished_blocks);
    return true;
}

#endif // debug

// Adding to the Queue:
// ====================

void cmd_queue_add_blocks(uint_fast8_t received_bytes)
{
    uint_fast8_t cur_position = 0;
    uint_fast8_t successfull_enqueued_commands = 0;
    uint_fast8_t block_bytes;
    uint_fast8_t cur_type;
    uint_fast8_t next_write_position;
    // Empty Parameter sends an OK reply. This is used by the host to check the Queue status.
    while(received_bytes > (cur_position + 1)) // received bytes is the number of bytes (starts at 1)
                                               // but cur_position is the index in those bytes (starts at 0)
    {
        // check if we have another Block
        if(BLOCK_ENVELOPE_SIZE > received_bytes - cur_position)
        {
            send_queue_failed_response(QUEUE_CAUSE_MALFORMED_BLOCK,
                                       successfull_enqueued_commands,
                                       DEFAULT_ERROR_REPLY_CODE,
                                       "too few bytes for a block envelope");
            return;
        }
        block_bytes = com_get_parameter_byte(cur_position);
        if(1 > block_bytes)
        {
            send_queue_failed_response(QUEUE_CAUSE_ERROR_IN_COMMAND_BLOCK,
                                       successfull_enqueued_commands,
                                       received_bytes,
                                       "block has no bytes");
            return;
        }
        if(cur_position + block_bytes > received_bytes)
        {
            send_queue_failed_response(QUEUE_CAUSE_ERROR_IN_COMMAND_BLOCK,
                                       successfull_enqueued_commands,
                                       DEFAULT_ERROR_REPLY_CODE,
                                       "inconsistent length");
            return;
        }
        cur_position++;
        cur_type = com_get_parameter_byte(cur_position);
        // check if Block length fits type
        switch(cur_type)
        {
        case MOVEMENT_BLOCK_TYPE_COMMAND_WRAPPER:
            // length dependent on wrapped command
            break;

        case MOVEMENT_BLOCK_TYPE_DELAY:
            if(3 != block_bytes)
            {
                send_queue_failed_response(QUEUE_CAUSE_MALFORMED_BLOCK,
                                           successfull_enqueued_commands,
                                           DEFAULT_ERROR_REPLY_CODE,
                                           "wrong number of bytes for Delay");
                return;
            }
            // else OK
            break;

        case MOVEMENT_BLOCK_TYPE_BASIC_LINEAR_MOVE:
            if(9 > block_bytes)
            {
                send_queue_failed_response(QUEUE_CAUSE_MALFORMED_BLOCK,
                                           successfull_enqueued_commands,
                                           DEFAULT_ERROR_REPLY_CODE,
                                           "too few bytes for Basic Linear Move");
                return;
            }
            // else Length OK
            break;

        case MOVEMENT_BLOCK_TYPE_SET_ACTIVE_TOOLHEAD:
            if(2 != block_bytes)
            {
                send_queue_failed_response(QUEUE_CAUSE_MALFORMED_BLOCK,
                                           successfull_enqueued_commands,
                                           DEFAULT_ERROR_REPLY_CODE,
                                           "wrong number of bytes for set active tool head");
                return;
            }
            // else OK
            break;

        case MOVEMENT_BLOCK_TYPE_MOVEMENT_CHECKPOINT:
            if(1 != block_bytes)
            {
                send_queue_failed_response(QUEUE_CAUSE_MALFORMED_BLOCK,
                                           successfull_enqueued_commands,
                                           DEFAULT_ERROR_REPLY_CODE,
                                           "wrong number of bytes for movement checkpoint");
                return;
            }
            break;

        default: // invalid type
            send_queue_failed_response(QUEUE_CAUSE_UNKNOWN_COMMAND_BLOCK,
                                       successfull_enqueued_commands,
                                       DEFAULT_ERROR_REPLY_CODE,
                                       "invalid block type");
            return;
        }

        // check if we have an open Slot
        next_write_position = write_pos +1;
        if(MAX_QUEUE_ELEMENTS == next_write_position)
        {
            // wrap around
            next_write_position = 0;
        }
        if(read_pos == next_write_position)
        {
            // Queue is full
            send_queue_failed_response(QUEUE_CAUSE_QUEUE_FULL,
                                       successfull_enqueued_commands,
                                       DEFAULT_ERROR_REPLY_CODE,
                                       "");
            return;
        }
        else
        {
            uint_fast8_t i;
            uint_fast8_t num_active_axis;
            uint_fast8_t bytes_per_axis;
            uint_fast8_t start_of_axis_steps;
            queue_type[write_pos] = cur_type;
            queue[write_pos][0] = block_bytes - 1;  // -1 removes the length byte
            for(i = 1; i < block_bytes ; i++)
            {
                queue[write_pos][i] = com_get_parameter_byte(cur_position + i);
            }
            // do some additional tests
            if(MOVEMENT_BLOCK_TYPE_BASIC_LINEAR_MOVE == cur_type)
            {
                if(AXIS_BYTE_MODE_SWITCH_MASK == (AXIS_BYTE_MODE_SWITCH_MASK & queue[write_pos][POSITION_OF_AXIS_BYTE_MODE_SWITCH]))
                {
                    // two Bytes for Axis Information
                    // CHECK: that we do not get steps for Axis 9 and above
                    // -> the first Bytes has only the not available Axis 9 to 14.
                    // -> so none of them are allowed to be active
                    if(AXIS_BYTE_MODE_SWITCH_MASK != queue[write_pos][POSITION_OF_AXIS_BYTE_MODE_SWITCH])
                    {
                        // we have an Active Axis that is higher than 8 !
                        send_queue_failed_response(QUEUE_CAUSE_MALFORMED_BLOCK,
                                                   successfull_enqueued_commands,
                                                   DEFAULT_ERROR_REPLY_CODE,
                                                   "axis index too high");
                        return;
                    }
                    // CHECK: that all active axis are also getting steps
                    // count the active Axis
                    num_active_axis = 0;
                    if(0x01 == (0x01 & queue[write_pos][2])) {num_active_axis++;}
                    if(0x02 == (0x02 & queue[write_pos][2])) {num_active_axis++;}
                    if(0x04 == (0x04 & queue[write_pos][2])) {num_active_axis++;}
                    if(0x08 == (0x08 & queue[write_pos][2])) {num_active_axis++;}
                    if(0x10 == (0x10 & queue[write_pos][2])) {num_active_axis++;}
                    if(0x20 == (0x20 & queue[write_pos][2])) {num_active_axis++;}
                    if(0x40 == (0x40 & queue[write_pos][2])) {num_active_axis++;}
                    if(0x80 == (0x80 & queue[write_pos][2])) {num_active_axis++;}
                    // 1 or 2 Bytes for Steps?
                    if(0x80 == (0x80 & queue[write_pos][3]))
                    {
                        bytes_per_axis = 2;
                        start_of_axis_steps = 11;
                    }
                    else
                    {
                        bytes_per_axis = 1;
                        start_of_axis_steps = 9;
                    }
                }
                else
                {
                    // one Byte for Axis Information
                    // CHECK: that all active axis are also getting steps
                    // count the active Axis
                    num_active_axis = 0;
                    if(0x01 == (0x01 & queue[write_pos][1])) {num_active_axis++;}
                    if(0x02 == (0x02 & queue[write_pos][1])) {num_active_axis++;}
                    if(0x04 == (0x04 & queue[write_pos][1])) {num_active_axis++;}
                    if(0x08 == (0x08 & queue[write_pos][1])) {num_active_axis++;}
                    if(0x10 == (0x10 & queue[write_pos][1])) {num_active_axis++;}
                    if(0x20 == (0x20 & queue[write_pos][1])) {num_active_axis++;}
                    if(0x40 == (0x40 & queue[write_pos][1])) {num_active_axis++;}
                    // 1 or 2 Bytes for Steps?
                    if(0x80 == (0x80 & queue[write_pos][2]))
                    {
                        bytes_per_axis = 2;
                        start_of_axis_steps = 9;
                    }
                    else
                    {
                        bytes_per_axis = 1;
                        start_of_axis_steps = 7;
                    }
                }
                // check the block length
                if(block_bytes < (num_active_axis * bytes_per_axis) + start_of_axis_steps)
                {
                    send_queue_failed_response(QUEUE_CAUSE_MALFORMED_BLOCK,
                                               successfull_enqueued_commands,
                                               DEFAULT_ERROR_REPLY_CODE,
                                               "invalid block length");
                    return;
                }
                // Check that Accel + Decel Steps are not higher than steps on primary axis
                // TODO
            }
            // Everything is OK
            write_pos = next_write_position;
            successfull_enqueued_commands++;
        }
        cur_position = cur_position + block_bytes;
    }
    send_queue_ok_response();
}

static void send_queue_failed_response(uint_fast8_t cause,
                                       uint_fast8_t enqueued_commands,
                                       uint_fast8_t error_reply_code,
                                       char* error_reason)
{
    uint_fast8_t i;
    uint_fast16_t slots_available;
    uint_fast16_t slots_taken;
    uint8_t *para_start = com_get_start_parameter();
    if(read_pos <= write_pos)
    {
        slots_taken = write_pos - read_pos;
    }
    else
    {
        slots_taken = MAX_QUEUE_ELEMENTS - read_pos + write_pos;
    }
    slots_available = MAX_QUEUE_ELEMENTS - slots_taken;

    para_start[0] = (uint8_t)cause;
    para_start[1] = (uint8_t)enqueued_commands;

    para_start[2] = (uint8_t)(slots_available >> 8) & 0xff;
    para_start[3] = (uint8_t)slots_available  & 0xff;

    para_start[4] = (uint8_t)(slots_taken >> 8) & 0xff;
    para_start[5] = (uint8_t)slots_taken & 0xff;

    para_start[6] = (uint8_t)(finished_blocks >> 8) & 0xff;
    para_start[7] = (uint8_t)finished_blocks & 0xff;
    para_start[8] = (uint8_t)error_reply_code;

    i = 0;
    while((0 != *error_reason) && (i < MAX_SEND_FRAME_SIZE - (9 + MIN_BYTES_CLIENT_FRAME)))
    {
        para_start[9 + i] = (uint8_t)*error_reason;
        i++;
        error_reason++;
    }
    com_send_order_specific_error_with_prefilled_parameter(9 + i);
}

static void send_queue_ok_response(void)
{
    uint_fast16_t slots_available;
    uint_fast16_t slots_taken;
    uint8_t *para_start = com_get_start_parameter();
    if(read_pos <= write_pos)
    {
        slots_taken = write_pos - read_pos;
    }
    else
    {
        slots_taken = MAX_QUEUE_ELEMENTS - read_pos + write_pos;
    }
    slots_available = MAX_QUEUE_ELEMENTS - slots_taken;

    para_start[0] = (uint8_t)(slots_available >> 8) & 0xff;
    para_start[1] = (uint8_t)slots_available  & 0xff;

    para_start[2] = (uint8_t)(slots_taken >> 8) & 0xff;
    para_start[3] = (uint8_t)slots_taken & 0xff;

    para_start[4] = (uint8_t)(finished_blocks >> 8) & 0xff;
    para_start[5] = (uint8_t)finished_blocks & 0xff;
    com_send_ok_with_prefilled_parameter(6);
}

#ifdef DEBUG_ACTIVE

bool cmd_queue_chnage_setting(uint8_t* setting)
{
    switch(*setting)
    {
#ifdef STEPPER_MODULE_TEST
    case 'E':
    case 'e': // enable stepper test
        if(true == enabled)
        {
            debug_line(STR("disabling !"));
            step_enable_motor(0, 0);
            enabled = false;
        }
        else
        {
            debug_line(STR("enabling !"));
            step_enable_motor(0, 1);
            enabled = true;
        }
        return true;

    case 'S':
    case 's': // set speed
        max_speed = atoi((char *)++setting);
        debug_line(STR("changing max speed to %d !"), max_speed);
        return true;

    case 'I':
    case 'i':
        if(true == enabled)
        {
            debug_line(STR("enabled !"));
        }
        else
        {
            debug_line(STR("disabled !"));
        }
        debug_line(STR("max speed is %d !"), max_speed);
        return true;

#else
#endif // STEPPER_MODULE_TEST
    default:
        return false;
    }
}

#endif

// removing from the Queue:
// ========================

void cmd_queue_tick(void)
{
#ifdef STEPPER_MODULE_TEST
    if(true == enabled)
    {
        if(false == step_is_busy())
        {
            // prepare next move
            if(start_speed < max_speed)
            {
                start_speed = start_speed + speed_increment;
            }
            else
            {
                if(start_speed > max_speed)
                {
                    // max speed was reduced
                    start_speed = start_speed - speed_increment;
                }
                else
                {
                    // start_speed is already max_speed
                    move_data[6] = 0;
                }
            }
            move_data[4] = start_speed;
            move_data[5] = start_speed;
            step_add_basic_linear_move(&move_data[0]);
        }
        // else step is busy -> so we can not send the next action over
    }
#else
#endif

    if(false == step_is_busy())
    {
        if(read_pos != write_pos)
        {
            // Queue has something -> execute that
            switch(queue_type[read_pos])
            {
            case MOVEMENT_BLOCK_TYPE_COMMAND_WRAPPER:
                handle_wrapped_command();
                finished_current_queued_block();
                break;

            case MOVEMENT_BLOCK_TYPE_DELAY:
                if(true == step_add_delay( (queue[read_pos][POS_OF_DELAY_MSB]<<8)
                                          + queue[read_pos][POS_OF_DELAY_LSB]     ) )
                {
                    finished_current_queued_block();
                }
                break;

            case MOVEMENT_BLOCK_TYPE_BASIC_LINEAR_MOVE:
                if(true == step_add_basic_linear_move(&queue[read_pos][0]))
                {
                    finished_current_queued_block();
                }
                break;

            case MOVEMENT_BLOCK_TYPE_SET_ACTIVE_TOOLHEAD:
                // TODO
                debug_line(STR("ERROR: not implemented Active Toolhead!"));
                // This is an extension point if a Firmware needs to do something
                // every time the Print head changes. This can then be implemented here.
                // Right now there is nothing to do here
                finished_current_queued_block();
                break;

            case MOVEMENT_BLOCK_TYPE_MOVEMENT_CHECKPOINT:
                // TODO
                debug_line(STR("ERROR: not implemented movement checkpoint!"));
                // This is an extension point if a Firmware needs to do something
                // every time the Print head changes. This can then be implemented here.
                // Right now there is nothing to do here
                finished_current_queued_block();
                break;

            default: // invalid type
                // TODO Event
                debug_line(STR("ERROR: invalid type!"));
                finished_current_queued_block();
                break;
            }

        }
        // else Queue is empty -> nothing to do.
    }
    // else step is busy -> so we can not send the next action over

}

static void handle_wrapped_command(void)
{
    switch(queue[read_pos][POS_OF_ORDER]) // Order
    {

    case ORDER_SET_HEATER_TARGET_TEMPERATURE:
        if(3 == queue[read_pos][POS_OF_LENGTH])
        {
            dev_heater_set_target_temperature(queue[read_pos][POS_OF_PARAMETER_START + 0],
                                             (queue[read_pos][POS_OF_PARAMETER_START + 1] <<8)
                                            + queue[read_pos][POS_OF_PARAMETER_START + 2] );
        }
        else
        {
            // TODO event
        }
        break;

    case ORDER_SWITCH_AN_OUTPUT:
        if((2 < queue[read_pos][POS_OF_LENGTH]) && (0 == queue[read_pos][POS_OF_LENGTH]%3))
        {
            uint_fast8_t bytes_done = 0;
            while(2 < queue[read_pos][POS_OF_LENGTH] - bytes_done)
            {
                switch(com_get_parameter_byte(queue[read_pos][POS_OF_PARAMETER_START + bytes_done + 0]))
                {

                case DEVICE_TYPE_SWITCH_OUTPUT:
                    dev_output_set_switch_state(queue[read_pos][POS_OF_PARAMETER_START + bytes_done + 1],
                                                queue[read_pos][POS_OF_PARAMETER_START + bytes_done + 2] );
                    break;

                case DEVICE_TYPE_SWITCH_INPUT:
                case DEVICE_TYPE_HEATER:
                case DEVICE_TYPE_TEMPERATURE_SENSOR:
                case DEVICE_TYPE_PWM_OUTPUT:
                case DEVICE_TYPE_BUZZER:
                case DEVICE_TYPE_STEPPER:
                    // TODO event
                    break;

                case DEVICE_TYPE_INVALID:
                default:
                    // TODO event
                    break;
                }
                bytes_done = bytes_done + 3;
            }
        }
        else
        {
            // TODO event
        }
        break;

    case ORDER_SET_PWM_OUTPUT_VALUE:
        if(4 == queue[read_pos][POS_OF_LENGTH])
        {
            oh_handle_set_pwm_output_value(queue[read_pos][POS_OF_PARAMETER_START + 0],
                                           queue[read_pos][POS_OF_PARAMETER_START + 1],
                                          (queue[read_pos][POS_OF_PARAMETER_START + 2] <<8) +
                                           queue[read_pos][POS_OF_PARAMETER_START + 3] );
        }
        else
        {
            // TODO event
        }
        break;

    case ORDER_ENABLE__DIABLE_STEPPER_MOTORS:
        if(2 == queue[read_pos][POS_OF_LENGTH])
        {
            dev_stepper_enable_motor(queue[read_pos][POS_OF_PARAMETER_START + 0],
                                     queue[read_pos][POS_OF_PARAMETER_START + 1]);
        }
        else if(0 == queue[read_pos][POS_OF_LENGTH])
        {
            dev_stepper_disable_all_motors();
        }
        else
        {
            // TODO event
        }
        break;

    case ORDER_SET_OUTPUT_TONE:
        if(4 == queue[read_pos][POS_OF_LENGTH])
        {
            oh_handle_set_output_tone(queue[read_pos][POS_OF_PARAMETER_START + 0],
                                      queue[read_pos][POS_OF_PARAMETER_START + 1],
                                     (queue[read_pos][POS_OF_PARAMETER_START + 2] <<8) +
                                      queue[read_pos][POS_OF_PARAMETER_START + 3] );
        }
        else
        {
            // TODO event
        }
        break;

    case ORDER_ENABLE__DISABLE_END_STOPS:
        if(0 ==  (queue[read_pos][POS_OF_LENGTH]-1)%2) // -1 is to remove the Order Byte
        {
            int i;
            for(i = 0; i < (queue[read_pos][POS_OF_LENGTH] -1)/2; i++)
            {
                if(true == dev_input_enable(queue[read_pos][POS_OF_PARAMETER_START + 0 + i*2],
                                            queue[read_pos][POS_OF_PARAMETER_START + 1 + i*2]) )
                {
                    // OK
                }
                else
                {
                    // TODO event
                    debug_line(STR("ERROR: failed to execute wrapped switch command!"));
                }
            }
        }
        else
        {
            // TODO event
            debug_line(STR("ERROR: invalid length of wrapped switch command(length: %d)!"), queue[read_pos][POS_OF_LENGTH]);
        }
        break;

    default: // invalid Order
        // TODO event
        debug_line(STR("ERROR: invalid wrapped command(Order : %d)!"), queue[read_pos][POS_OF_ORDER]);
        break;
    }
}

static void finished_current_queued_block(void)
{
    read_pos++;
    if(MAX_QUEUE_ELEMENTS == read_pos)
    {
        read_pos = 0;
    }
    finished_blocks++;
}

// end of File
