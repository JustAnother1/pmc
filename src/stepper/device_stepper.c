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
#include "device_buzzer.h"
#include "protocol.h"
#include "com.h"
#include "step.h"
#include "device_input.h"
#include "trinamic.h"
#include "hal_cfg.h"
#include "hal_spi.h"

//End Stops:
#define MAX_END  1
#define MIN_END  0

static char Stepper_name[]= "Stepper 0";

uint_fast8_t available_steppers;
uint_fast8_t state[MAX_NUMBER];
uint_fast8_t max_end_stop[MAX_NUMBER];
uint_fast8_t min_end_stop[MAX_NUMBER];
uint_fast32_t max_steps_per_second[MAX_NUMBER];
uint_fast32_t underrun_max_steps_per_second[MAX_NUMBER];
uint_fast32_t underrun_max_decelleration[MAX_NUMBER];

void dev_stepper_init(void)
{
    int i = 0;
    for(i = 0; i < MAX_NUMBER; i++)
    {
        state[i] = DEVICE_STATUS_FAULT;
        max_end_stop[i] = 0;
        min_end_stop[i] = 0;
    }
    trinamic_init();
    available_steppers = trinamic_detect_number_of_steppers();
    if((0 < available_steppers) && (MAX_NUMBER+1 > available_steppers))
    {
        trinamic_configure_steppers(available_steppers);
    }
    step_init(available_steppers);
    for(i = 0; i < available_steppers; i++)
    {
        state[i] = DEVICE_STATUS_ACTIVE;
    }
}

uint_fast8_t dev_stepper_get_count(void)
{
    return available_steppers;
}

uint_fast8_t dev_stepper_get_name(uint_fast8_t number, uint8_t *position)
{
    if(number > available_steppers -1)
    {
        return 0;
    }
    else
    {
        uint_fast8_t length = com_copy_string_to_parameter((char*)&Stepper_name, position);
        position = position + length -1;
        // increment the number at the end of the String
        *position = *position + number;
        return length;
    }
}

uint_fast8_t dev_stepper_get_status(uint_fast8_t number)
{
    if(number > available_steppers -1)
    {
        return DEVICE_STATUS_FAULT;
    }
    else
    {
        return state[number];
    }
}

void dev_stepper_activate(uint_fast8_t on_off)
{
    // TODO do the initialization of the stepper motors after being activated
    if(0 == on_off)
    {
        // Stepper controlled by someone else
        com_send_ok_response();
    }
    else if (1 == on_off)
    {
        // stepper controlled by this firmware
        com_send_ok_response();
    }
    else
    {
        // invalid Value
        com_send_generic_application_error_response(GENERIC_ERROR_BAD_PARAMETER_VALUE);
    }
}

void dev_stepper_disable_all_motors(void)
{
    step_disable_all_motors();
}

void dev_stepper_enable_motor(uint_fast8_t stepper_number, uint_fast8_t on_off)
{
    step_enable_motor(stepper_number, on_off);
}

void dev_stepper_configure_end_stops(uint_fast8_t stepper_number, uint_fast8_t switch_number, uint_fast8_t min_max)
{
    if(stepper_number > available_steppers -1)
    {
        com_send_generic_application_error_response(GENERIC_ERROR_INVALID_DEVICE_NUMBER);
    }
    else
    {
        if(switch_number < dev_input_get_count())
        {
            if(MAX_END == min_max)
            {
                max_end_stop[stepper_number] = switch_number;
                com_send_ok_response();
            }
            else if( MIN_END == min_max)
            {
                min_end_stop[stepper_number] = switch_number;
                com_send_ok_response();
            }
            else
            {
                com_send_generic_application_error_response(GENERIC_ERROR_BAD_PARAMETER_VALUE);
            }
        }
        else
        {
            com_send_generic_application_error_response(GENERIC_ERROR_BAD_PARAMETER_VALUE);
        }
    }
}

void dev_stepper_configure_both_end_stops(uint_fast8_t stepper_number, uint_fast8_t switch_number, uint_fast8_t min_max,
                                                                       uint_fast8_t switch_number2, uint_fast8_t min_max2)
{
    if(stepper_number > available_steppers -1)
    {
        com_send_generic_application_error_response(GENERIC_ERROR_INVALID_DEVICE_NUMBER);
        return;
    }
    // first Switch
    if(switch_number < dev_input_get_count())
    {
        if(MAX_END == min_max)
        {
            max_end_stop[stepper_number] = switch_number;
        }
        else if( MIN_END == min_max)
        {
            min_end_stop[stepper_number] = switch_number;
        }
        else
        {
            com_send_generic_application_error_response(GENERIC_ERROR_BAD_PARAMETER_VALUE);
            return;
        }
    }
    else
    {
        com_send_generic_application_error_response(GENERIC_ERROR_BAD_PARAMETER_VALUE);
        return;
    }

    // second switch
    if(switch_number2 < dev_input_get_count())
    {
        if(MAX_END == min_max2)
        {
            max_end_stop[stepper_number] = switch_number2;
        }
        else if( MIN_END == min_max2)
        {
            min_end_stop[stepper_number] = switch_number2;
        }
        else
        {
            com_send_generic_application_error_response(GENERIC_ERROR_BAD_PARAMETER_VALUE);
            return;
        }
    }
    else
    {
        com_send_generic_application_error_response(GENERIC_ERROR_BAD_PARAMETER_VALUE);
        return;
    }
    com_send_ok_response();
}

void dev_stepper_configure_axis_movement_rate(uint_fast8_t stepper_number, uint_fast32_t max_steps)
{
    if(stepper_number > available_steppers -1)
    {
        com_send_generic_application_error_response(GENERIC_ERROR_INVALID_DEVICE_NUMBER);
    }
    else
    {
        if((0 < max_steps) && ( MAX_STEP_RATE+1 > max_steps))
        {
            max_steps_per_second[stepper_number -1] = max_steps;
            com_send_ok_response();
        }
        else
        {
            com_send_generic_application_error_response(GENERIC_ERROR_BAD_PARAMETER_VALUE);
        }
    }
}

void dev_stepper_configure_mvmnt_unrun_avoid_para(uint_fast8_t stepper_number,
                                                  uint_fast32_t max_steps,
                                                  uint_fast32_t max_decelleration)
{
    if(stepper_number > available_steppers -1)
    {
        com_send_generic_application_error_response(GENERIC_ERROR_INVALID_DEVICE_NUMBER);
    }
    else
    {
        if((0 < max_steps) && ( MAX_STEP_RATE+1 > max_steps))
        {
            if(0 < max_decelleration)
            {
                underrun_max_steps_per_second[stepper_number -1] = max_steps;
                underrun_max_decelleration[stepper_number -1] = max_decelleration;
                com_send_ok_response();
            }
            else
            {
                com_send_generic_application_error_response(GENERIC_ERROR_BAD_PARAMETER_VALUE);
            }
        }
        else
        {
            com_send_generic_application_error_response(GENERIC_ERROR_BAD_PARAMETER_VALUE);
        }
    }
}

// end of File
