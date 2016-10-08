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
#include "hal_din.h"
#include "endStopHandling.h"

void dev_input_init(void)
{
    hal_din_init();
}

uint_fast8_t dev_input_get_count(void)
{
    return hal_din_get_amount();
}

uint_fast8_t dev_input_get_status(uint_fast8_t number)
{
    if(number < hal_din_get_amount())
    {
    	if(false == dev_input_is_enabled(number))
    	{
    		return DEVICE_STATUS_DISABLED;
    	}
    	else
    	{
    		return DEVICE_STATUS_ACTIVE;
    	}
    }
    else
    {
        return DEVICE_STATUS_FAULT;
    }
}

uint_fast8_t dev_input_get_name(uint_fast8_t number, uint8_t *position)
{
    return hal_din_get_name(number, position);
}

uint_fast8_t dev_input_get_switch_state(uint_fast8_t number)
{
    // TODO handle inverted switch
    return hal_din_get_switch_state(number);
}

// end of File
