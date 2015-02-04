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

#include "command_queue.h"
#include "step.h"
#include "protocol.h"
#include "com.h"
#include "device_heater.h"
#include "device_output.h"
#include "orderhandler.h"
#include "device_stepper.h"
#include "device_input.h"

#define MAX_QUEUE_ELEMENTS                10
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
static void send_queue_failed_response(uint_fast8_t cause, uint_fast8_t enqueued_commands);
static void handle_wrapped_command(void);
static void finished_current_stacked_block(void);

uint_fast8_t queue[MAX_QUEUE_ELEMENTS][MAX_BLOCK_LENGTH];
uint_fast8_t queue_type[MAX_QUEUE_ELEMENTS];
// Index to Queue for:
uint_fast8_t read_pos; // reading
uint_fast8_t write_pos; // writing
uint_fast8_t stacked_pos; // start position of skipped and not executed commands
uint_fast8_t stacked_non_move_orders = 0; // number of commands that have been skipped
uint_fast16_t finished_blocks =0; // number of successfully executed Blocks
bool move_since_last_tag = false;

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
    while(received_bytes - cur_position > 0)
    {
        // check if we have another Block
        if(BLOCK_ENVELOPE_SIZE > received_bytes - cur_position)
        {
            // too few bytes for a block
            send_queue_failed_response(QUEUE_CAUSE_MALFORMED_BLOCK, successfull_enqueued_commands);
            return;
        }
        block_bytes = com_get_parameter_byte(cur_position);
        if(1 > block_bytes)
        {
            // too few bytes for a block
            send_queue_failed_response(QUEUE_CAUSE_ERROR_IN_COMMAND_BLOCK, successfull_enqueued_commands);
            return;
        }
        if(cur_position + block_bytes > received_bytes)
        {
            // something is broken in the data
            send_queue_failed_response(QUEUE_CAUSE_ERROR_IN_COMMAND_BLOCK, successfull_enqueued_commands);
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
                send_queue_failed_response(QUEUE_CAUSE_MALFORMED_BLOCK, successfull_enqueued_commands);
                return;
            }
            // else OK
            break;

        case MOVEMENT_BLOCK_TYPE_BASIC_LINEAR_MOVE:
            if(9 > block_bytes)
            {
                send_queue_failed_response(QUEUE_CAUSE_MALFORMED_BLOCK, successfull_enqueued_commands);
                return;
            }
            // else Length OK
            break;

        case MOVEMENT_BLOCK_TYPE_SET_ACTIVE_TOOLHEAD:
            if(2 != block_bytes)
            {
                send_queue_failed_response(QUEUE_CAUSE_MALFORMED_BLOCK, successfull_enqueued_commands);
                return;
            }
            // else OK
            break;

        case MOVEMENT_BLOCK_TYPE_MOVEMENT_CHECKPOINT:
            if(1 != block_bytes)
            {
                send_queue_failed_response(QUEUE_CAUSE_MALFORMED_BLOCK, successfull_enqueued_commands);
                return;
            }
            break;

        default: // invalid type
            send_queue_failed_response(QUEUE_CAUSE_UNKNOWN_COMMAND_BLOCK, successfull_enqueued_commands);
            return;
        }

        // check if we have an open Slot
        next_write_position = write_pos +1;
        if(MAX_QUEUE_ELEMENTS == next_write_position)
        {
            // wrap around
            next_write_position = 0;
        }
        if((read_pos == next_write_position) || ((0 != stacked_non_move_orders) && (stacked_pos == next_write_position)))
        {
            // Queue is full
            send_queue_failed_response(QUEUE_CAUSE_QUEUE_FULL, successfull_enqueued_commands);
            return;
        }
        else
        {
            uint_fast8_t i;
            uint_fast8_t num_active_axis;
            uint_fast8_t bytes_per_axis;
            uint_fast8_t start_of_axis_steps;
            cur_position++;
            queue_type[write_pos] = cur_type;
            queue[write_pos][0] = block_bytes -1;
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
                        send_queue_failed_response(QUEUE_CAUSE_MALFORMED_BLOCK, successfull_enqueued_commands);
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
                    // check the block length
                    if(block_bytes < (num_active_axis * bytes_per_axis) + start_of_axis_steps)
                    {
                        send_queue_failed_response(QUEUE_CAUSE_MALFORMED_BLOCK, successfull_enqueued_commands);
                        return;
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
                    // check the block length
                    if(block_bytes < (num_active_axis * bytes_per_axis) + start_of_axis_steps)
                    {
                        send_queue_failed_response(QUEUE_CAUSE_MALFORMED_BLOCK, successfull_enqueued_commands);
                        return;
                    }
                }
            }
            // Everything is OK
            write_pos = next_write_position;
            successfull_enqueued_commands++;
        }
        cur_position = cur_position + block_bytes - 1;
    }
    send_queue_ok_response();
}

static void send_queue_failed_response(uint_fast8_t cause, uint_fast8_t enqueued_commands)
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

    para_start[0] = (uint8_t)cause;
    para_start[1] = (uint8_t)enqueued_commands;

    para_start[2] = (uint8_t)(slots_available >> 8) & 0xff;
    para_start[3] = (uint8_t)slots_available  & 0xff;

    para_start[4] = (uint8_t)(slots_taken >> 8) & 0xff;
    para_start[5] = (uint8_t)slots_taken & 0xff;

    para_start[6] = (uint8_t)(finished_blocks >> 8) & 0xff;
    para_start[7] = (uint8_t)finished_blocks & 0xff;
    para_start[8] = (uint8_t)0xff;
    com_send_order_specific_error_with_prefilled_parameter(9);
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

    para_start[2] = (uint8_t)(finished_blocks >> 8) & 0xff;
    para_start[3] = (uint8_t)finished_blocks & 0xff;
    com_send_ok_with_prefilled_parameter(6);
}

// removing from the Queue:
// ========================

void cmd_queue_tick(void)
{
    if(0 < stacked_non_move_orders)
    {
        if(true == step_has_reached_tag())
        {
            // execute all stacked Orders
            uint_fast8_t i;
            for(i = 0; i < stacked_non_move_orders; i++)
            {
                // Queue has something -> execute that
                switch(queue_type[stacked_pos])
                {
                case MOVEMENT_BLOCK_TYPE_COMMAND_WRAPPER:
                    handle_wrapped_command();
                    break;

                default: // invalid type
                    // TODO Event
                    break;
                }
                finished_current_stacked_block();
            }
            stacked_non_move_orders = 0;
        }
        // else Tag not reached yet -> wait for it.
    }
    if(false == step_is_busy())
    {
        if(read_pos != write_pos)
        {
            // Queue has something -> execute that
            switch(queue_type[read_pos])
            {
            case MOVEMENT_BLOCK_TYPE_COMMAND_WRAPPER:
                if(0 == stacked_non_move_orders)
                {
                    // first non move command
                    step_request_tag();
                    stacked_non_move_orders = 1;
                    stacked_pos = read_pos;
                    move_since_last_tag = false;
                    finished_current_queued_block();
                }
                else
                {
                    if(false == move_since_last_tag)
                    {
                        stacked_non_move_orders++;
                        finished_current_queued_block();
                    }
                    // else we can not execute this until the Tag has been reached
                    // -> same as queue Busy - no finishedCurrentQueuedBlock();
                }
                break;

            case MOVEMENT_BLOCK_TYPE_DELAY:
                move_since_last_tag = true;
                if(true == step_add_delay(queue[read_pos][POS_OF_DELAY_MSB], queue[read_pos][POS_OF_DELAY_LSB]))
                {
                    finished_current_queued_block();
                }
                break;

            case MOVEMENT_BLOCK_TYPE_BASIC_LINEAR_MOVE:
                move_since_last_tag = true;
                if(true == step_add_basic_linear_move(&queue[read_pos][0]))
                {
                    finished_current_queued_block();
                }
                break;

            case MOVEMENT_BLOCK_TYPE_SET_ACTIVE_TOOLHEAD:
                move_since_last_tag = true;
                // TODO
                // This is an extension point if a Firmware needs to do something
                // every time the Print head changes. This can then be implemented here.
                // Right now there is nothing to do here
                finished_current_queued_block();
                break;

            case MOVEMENT_BLOCK_TYPE_MOVEMENT_CHECKPOINT:
                move_since_last_tag = true;
                // TODO
                // This is an extension point if a Firmware needs to do something
                // every time the Print head changes. This can then be implemented here.
                // Right now there is nothing to do here
                finished_current_queued_block();
                break;

            default: // invalid type
                // TODO Event
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
        if(2 == queue[read_pos][POS_OF_LENGTH])
        {
            // one Switch
            dev_input_enable(queue[read_pos][POS_OF_PARAMETER_START + 0],
                             queue[read_pos][POS_OF_PARAMETER_START + 1]);
        }
        else if(4 == queue[read_pos][POS_OF_LENGTH])
        {
            // two switches
            dev_input_enable_two(queue[read_pos][POS_OF_PARAMETER_START + 0],
                                 queue[read_pos][POS_OF_PARAMETER_START + 1],
                                 queue[read_pos][POS_OF_PARAMETER_START + 2],
                                 queue[read_pos][POS_OF_PARAMETER_START + 3] );
        }
        else
        {
            // TODO event
        }
        break;

    default: // invalid Order
        // TODO event
        break;
    }
}

static void finished_current_stacked_block(void)
{
    stacked_pos++;
    if(MAX_QUEUE_ELEMENTS == stacked_pos)
    {
        stacked_pos = 0;
    }
    // finished_blocks++; Block has already been counted when put into Stack
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
