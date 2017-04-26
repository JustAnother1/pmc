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

#include "step.h"
#include "protocol.h"
#include "end_stop_handling.h"
#include "hal_cfg.h"
#include "hal_cpu.h"
#include "hal_din.h"
#include "hal_debug.h"
#include "com.h"

// index is stepper number, value in array is switch number
static uint_fast8_t max_end_stop[MAX_NUMBER];
static uint_fast8_t min_end_stop[MAX_NUMBER];
// index is switch number, value in array is if this switch is active or not
static volatile uint_fast8_t volatile enabled[MAX_NUM_END_STOPS];
static uint32_t debounce[MAX_NUM_END_STOPS];
static bool is_triggered[MAX_NUM_END_STOPS];
static bool inverted[MAX_NUM_END_STOPS]; // TODO make this configurable

void endStopHandling_init(void)
{
    int i;
    for(i = 0; i < MAX_NUMBER; i++)
    {
        max_end_stop[i] = 0xff;
        min_end_stop[i] = 0xff;
    }
    for(i = 0; i < MAX_NUM_END_STOPS; i++)
    {
        enabled[i] = END_STOP_DISABLED;
        is_triggered[i] = false;
        inverted[i] = true;
    }
}

uint_fast8_t dev_input_get_switch_state(uint_fast8_t number)
{
    uint_fast8_t level = hal_din_get_switch_state(number);
    if(DIN_HIGH == level)
    {
        if(false == inverted[number])
        {
            return DIN_HIGH;
        }
        else
        {
            return DIN_LOW;
        }
    }
    else if(DIN_LOW == level)
    {
        if(false == inverted[number])
        {
            return DIN_LOW;
        }
        else
        {
            return DIN_HIGH;
        }
    }
    else
    {
        return DIN_ERROR;
    }
}

bool dev_stepper_is_end_stop_triggered(uint_fast8_t stepper, uint_fast8_t min_max)
{
    if(MAX_END == min_max)
    {
        if(0xff == max_end_stop[stepper])
        {
            // that end stop is not available -> therefore not triggered
            return false;
        }
        else
        {
            uint_fast8_t level = dev_input_get_switch_state(max_end_stop[stepper]);
            if(DIN_HIGH == level)
            {
                debug_line(STR("triggered %d"), max_end_stop[stepper]);
                return true;
            }
            else // DIN_LOW or DIN_ERROR
            {
                return false;
            }
        }
    }
    else if( MIN_END == min_max)
    {
        if(0xff == min_end_stop[stepper])
        {
            // that end stop is not available -> therefore not triggered
            return false;
        }
        else
        {
            uint_fast8_t level = dev_input_get_switch_state(min_end_stop[stepper]);
            if(DIN_HIGH == level)
            {
                return true;
            }
            else // DIN_LOW or DIN_ERROR
            {
                return false;
            }
        }
    }
    else
    {
        // end stop is neither min nor max? that is wrong!
        // -> therefore triggered to be safe.
        debug_line(STR("ERROR"));
        return true;
    }
}

static void handle_end_stop_triggered(bool high, uint_fast8_t stepper, uint_fast8_t switch_number)
{
    if(END_STOP_ENABLED == enabled[switch_number])
    {
        if(   ((false == inverted[switch_number]) && (true == high))
           || ((true  == inverted[switch_number]) && (false == high)) )
        {
            // end stop has triggered
            if(false == is_triggered[switch_number])
            {
                is_triggered[switch_number] = true;
                if(true == step_is_homing())
                {
                    // stop movement on this stepper
                    debug_line(STR("End Stop %d hit -> stop movement"), switch_number);
                    if(switch_number == max_end_stop[stepper])
                    {
                        step_end_stop_hit_on(stepper, true);
                    }
                    else if(switch_number == min_end_stop[stepper])
                    {
                        step_end_stop_hit_on(stepper, false);
                    }
                    else
                    {
                        // -> something is wrong !
                        debug_line(STR("End Stop %d hit but is not associated with stepper %d -> go to stopped mode!"), switch_number, stepper);
                        gotoStoppedMode(STOPPED_CAUSE_END_STOP_HIT, RECOVERY_CONDITION_CLEARED);
                    }
                }
                else
                {
                    // end stop was hit unexpectedly and was enabled ! -> we have a problem !
                    debug_line(STR("End Stop %d hit -> go to stopped mode!"), switch_number);
                    gotoStoppedMode(STOPPED_CAUSE_END_STOP_HIT, RECOVERY_CONDITION_CLEARED);
                }
                debounce[switch_number] = hal_cpu_get_ms_tick() + 5;
            }
            // else already detected (bouncing switch)
        }
        else
        {
            // end stop is not triggered any more
            if(true == is_triggered[switch_number])
            {
                uint32_t now =  hal_cpu_get_ms_tick();
                if(now > debounce[switch_number])
                {
                    is_triggered[switch_number] = false;
                }
                // else switch is bouncing
            }
            debug_line(STR("End Stop %d not triggered(inverted : %d, high: %d))"),
                        switch_number, inverted[switch_number], high);
        }
    }
    else
    {
        // else the end stop is not enabled -> we ignore this event
        debug_line(STR("End Stop %d not enabled!"), switch_number);
    }
}

bool dev_input_enable(uint_fast8_t switch_number, uint_fast8_t enable)
{
    if(switch_number < MAX_NUM_END_STOPS)
    {
        if(END_STOP_ENABLED == enable)
        {
            debug_line(STR("enabling end stop %d"), switch_number);
            enabled[switch_number] = END_STOP_ENABLED;
            return true;
        }
        else if(END_STOP_DISABLED == enable)
        {
            debug_line(STR("disabling end stop %d"), switch_number);
            enabled[switch_number] = END_STOP_DISABLED;
            return true;
        }
        else
        {
            com_send_generic_application_error_response(GENERIC_ERROR_BAD_PARAMETER_VALUE);
            return false;
        }
    }
    else
    {
        com_send_generic_application_error_response(GENERIC_ERROR_INVALID_DEVICE_NUMBER);
        return false;
    }
}

bool dev_input_is_enabled(uint_fast8_t switch_number)
{
    if(switch_number < MAX_NUM_END_STOPS)
    {
        if(END_STOP_ENABLED == enabled[switch_number])
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        // a non available switch it not enabled.
        return false;
    }
}

bool dev_stepper_configure_end_stops(uint_fast8_t stepper_number, uint_fast8_t switch_number, uint_fast8_t min_max)
{
    if(stepper_number -1  > MAX_NUMBER)
    {
        com_send_generic_application_error_response(GENERIC_ERROR_INVALID_DEVICE_NUMBER);
        return false;
    }
    else
    {
        if(switch_number < MAX_NUM_END_STOPS)
        {
            if(MAX_END == min_max)
            {
                debug_line(STR("Stepper %d Max is Switch %d!"), stepper_number, switch_number);
                hal_din_subscribe_to_events(switch_number, stepper_number, handle_end_stop_triggered);
                max_end_stop[stepper_number] = switch_number;
                return true;
            }
            else if( MIN_END == min_max)
            {
                debug_line(STR("Stepper %d Min is Switch %d!"), stepper_number, switch_number);
                hal_din_subscribe_to_events(switch_number, stepper_number, handle_end_stop_triggered);
                min_end_stop[stepper_number] = switch_number;
                return true;
            }
            else
            {
                com_send_generic_application_error_response(GENERIC_ERROR_BAD_PARAMETER_VALUE);
                return false;
            }
        }
        else
        {
            com_send_generic_application_error_response(GENERIC_ERROR_BAD_PARAMETER_VALUE);
            return false;
        }
    }
}



