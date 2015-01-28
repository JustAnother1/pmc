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
#include "hal_dout.h"
#include "hal_debug.h"

void dev_output_init(void)
{
    hal_dout_init();
}

uint_fast8_t dev_output_get_count(void)
{
    return hal_dout_get_amount();
}

uint_fast8_t dev_output_get_name(uint_fast8_t number, uint8_t *position)
{
    return hal_dout_get_name(number, position);
}

uint_fast8_t dev_output_get_status(uint_fast8_t number)
{
    if(number < hal_dout_get_amount())
    {
        return DEVICE_STATUS_ACTIVE;
    }
    else
    {
        return DEVICE_STATUS_FAULT;
    }
}

uint_fast8_t dev_output_get_switch_state(uint_fast8_t number)
{
    // TODO
    return SWITCH_STATE_NOT_TRIGGERED;
}

void dev_output_set_switch_state(uint_fast8_t number, uint_fast8_t state)
{
    if(number < hal_dout_get_amount())
    {
        switch(state)
        {
        case OUTPUT_STATE_LOW:
            hal_dout_set_pin_low(number);
            break;

        case OUTPUT_STATE_HIGH:
            hal_dout_set_pin_high(number);
            break;

        case OUTPUT_STATE_HIGH_Z:
            // TODO
            debug_line("high-Z:not implemented");
            break;

        default:
            debug_line("invalid output state (%d)!", state);
            break;
        }
    }
    else
    {
        debug_line("requested switching of invalid output(%d)!", number);
    }
}

// end of File
