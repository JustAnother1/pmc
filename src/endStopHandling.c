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
#include "endStopHandling.h"
#include "com.h"

//End Stops:
#define MAX_END  1
#define MIN_END  0
#define MAX_NUM_END_STOPS  6
#define END_STOP_ENABLED  1
#define END_STOP_DISABLED 0

static uint_fast8_t max_end_stop[MAX_NUMBER];
static uint_fast8_t min_end_stop[MAX_NUMBER];
static bool enabled[MAX_NUM_END_STOPS];

void endStopHandling_init(void)
{
	int i;
    for(i = 0; i < MAX_NUMBER; i++)
    {
        max_end_stop[i] = 0;
        min_end_stop[i] = 0;
    }
    for(i = 0; i < MAX_NUM_END_STOPS; i++)
    {
    	enabled[i] = END_STOP_DISABLED;
    }
    // TODO: Activate ISR that listens to all enabled Switches
    // If an enabled Switch goes into the triggered state while not in a homing move then:
    // - halt all movement
    // - deactivate al heaters
    // - disable all outputs
    // - flush the order queue
    // - go into stopped state
}


bool dev_input_enable(uint_fast8_t switch_number, uint_fast8_t enable)
{
	if(switch_number < MAX_NUM_END_STOPS)
	{
		if(END_STOP_ENABLED == enable)
		{
			enabled[switch_number] = true;
			return true;
		}
		else if(END_STOP_DISABLED == enable)
		{
			enabled[switch_number] = false;
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
                max_end_stop[stepper_number] = switch_number;
                return true;
            }
            else if( MIN_END == min_max)
            {
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



