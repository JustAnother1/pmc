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
#include <stdbool.h>
#include "board_cfg.h"
#include "device_buzzer.h"
#include "protocol.h"
#include "com.h"
#include "step.h"
#include "device_input.h"
#include "hal_cfg.h"
#include "hal_spi.h"
#include "device_stepper.h"
#include "copy_string.h"
#ifdef HAS_TRINAMIC
#include "trinamic.h"
#else
#include "pololu.h"
#endif

#ifdef NAMED_STEPPERS
#else
static char Stepper_name[]= "Stepper 0";
#endif

static uint_fast8_t state[MAX_NUMBER];
static uint_fast32_t max_steps_per_second[MAX_NUMBER];
static uint_fast32_t underrun_max_steps_per_second[MAX_NUMBER];
static uint_fast32_t underrun_max_decelleration[MAX_NUMBER];
static bool weControllTheSteppers = true;

#ifdef HAS_TRINAMIC
static uint_fast8_t available_steppers;
#else
#define available_steppers NUMBER_OF_STEPPERS
#endif

void dev_stepper_init(void)
{
    int i = 0;
    for(i = 0; i < MAX_NUMBER; i++)
    {
        state[i] = DEVICE_STATUS_FAULT;
        max_steps_per_second[i] = 0;
        underrun_max_steps_per_second[i] = 0;
        underrun_max_decelleration[i] = 0;
    }
#ifdef HAS_TRINAMIC
    available_steppers = 0;
#else
    step_init(available_steppers);
    for(i = 0; i < available_steppers; i++)
    {
        state[i] = DEVICE_STATUS_ACTIVE;
    }
    pololu_init();
#endif
}

uint_fast8_t dev_stepper_get_count(void)
{
    if((true == weControllTheSteppers) && (0 == available_steppers))
    {
#ifdef HAS_TRINAMIC
        trinamic_init();
        dev_stepper_detectSteppers();
        return available_steppers;
#else
        return 0;
#endif
    }
    else
    {
        return available_steppers;
    }
}

uint_fast8_t dev_stepper_get_name(uint_fast8_t number, uint8_t *position, uint_fast8_t max_length)
{
#ifdef HAS_TRINAMIC
    if((true == weControllTheSteppers) && (0 == available_steppers))
    {
        trinamic_init();
        dev_stepper_detectSteppers();
    }
#endif
    if(number -1 > available_steppers)
    {
        return 0;
    }
    else
    {
#ifdef NAMED_STEPPERS
        switch(number)
        {
    #if NUMBER_OF_STEPPERS > 0
        case  0: return strlcpy_P((char *)position, STR(STEPPER_0_NAME), max_length);
    #endif
    #if NUMBER_OF_STEPPERS > 1
        case  1: return strlcpy_P((char *)position, STR(STEPPER_1_NAME), max_length);
    #endif
    #if NUMBER_OF_STEPPERS > 2
        case  2: return strlcpy_P((char *)position, STR(STEPPER_2_NAME), max_length);
    #endif
    #if NUMBER_OF_STEPPERS > 3
        case  3: return strlcpy_P((char *)position, STR(STEPPER_3_NAME), max_length);
    #endif
    #if NUMBER_OF_STEPPERS > 4
        case  4: return strlcpy_P((char *)position, STR(STEPPER_4_NAME), max_length);
    #endif
    #if NUMBER_OF_STEPPERS > 5
        case  5: return strlcpy_P((char *)position, STR(STEPPER_5_NAME), max_length);
    #endif
    #if NUMBER_OF_STEPPERS > 6
        case  6: return strlcpy_P((char *)position, STR(STEPPER_6_NAME), max_length);
    #endif
    #if NUMBER_OF_STEPPERS > 7
        case  7: return strlcpy_P((char *)position, STR(STEPPER_7_NAME), max_length);
    #endif
        default:
            return 0;
        }
#else
        uint_fast8_t length = copy_string((char*)&Stepper_name, position, max_length);
        position = position + length -1;
        // increment the number at the end of the String
        *position = *position + number;
        return length;
#endif
    }
}

uint_fast8_t dev_stepper_get_status(uint_fast8_t number)
{
#ifdef HAS_TRINAMIC
    if((true == weControllTheSteppers) && (0 == available_steppers))
    {
        trinamic_init();
        dev_stepper_detectSteppers();
    }
#endif
    if(number -1 > available_steppers)
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
    if(0 == on_off)
    {
        // Stepper controlled by someone else
        weControllTheSteppers = false;
#ifdef HAS_TRINAMIC
        available_steppers = 0;
#endif
        com_send_ok_response();
    }
    else if (1 == on_off)
    {
        // stepper controlled by this firmware
        weControllTheSteppers = true;
#ifdef HAS_TRINAMIC
        trinamic_init();
        available_steppers = 0;
        dev_stepper_detectSteppers();
#endif
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
#ifdef HAS_TRINAMIC
    if((true == weControllTheSteppers) && (0 == available_steppers))
    {
        trinamic_init();
        dev_stepper_detectSteppers();
    }
#endif
    if(true == weControllTheSteppers)
    {
        step_disable_all_motors();
    }
    // else -> nothing to do
}

void dev_stepper_enable_motor(uint_fast8_t stepper_number, uint_fast8_t on_off)
{
#ifdef HAS_TRINAMIC
    if((true == weControllTheSteppers) && (0 == available_steppers))
    {
        trinamic_init();
        dev_stepper_detectSteppers();
    }
#endif
    if(true == weControllTheSteppers)
    {
        step_enable_motor(stepper_number, on_off);
    }
    // else -> nothing to do
}

void dev_stepper_configure_axis_movement_rate(uint_fast8_t stepper_number, uint_fast32_t max_steps)
{
#ifdef HAS_TRINAMIC
    if((true == weControllTheSteppers) && (0 == available_steppers))
    {
        trinamic_init();
        dev_stepper_detectSteppers();
    }
#endif
    if(stepper_number -1 > available_steppers)
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
#ifdef HAS_TRINAMIC
    if((true == weControllTheSteppers) && (0 == available_steppers))
    {
        trinamic_init();
        dev_stepper_detectSteppers();
    }
#endif
    if(stepper_number -1 > available_steppers)
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

#ifdef HAS_TRINAMIC
void dev_stepper_detectSteppers(void)
{
    unsigned int i;
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
#endif

// end of File
