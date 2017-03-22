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
#include "hal_buzzer.h"

void dev_buzzer_init(void)
{
    hal_buzzer_init();
}

uint_fast8_t dev_buzzer_get_count(void)
{
    return hal_buzzer_get_amount();
}

uint_fast8_t dev_buzzer_get_name(uint_fast8_t number, uint8_t *position, uint_fast8_t max_length)
{
    return hal_buzzer_get_name(number, position, max_length);
}

uint_fast8_t dev_buzzer_get_status(uint_fast8_t number)
{
    if(number < hal_buzzer_get_amount())
    {
        return DEVICE_STATUS_ACTIVE;
    }
    else
    {
        return DEVICE_STATUS_FAULT;
    }
}

void dev_buzzer_set_pwm(uint_fast8_t number, uint_fast16_t pwm)
{
    // TODO. Really ?
	(void) number;
	(void) pwm;
}

void dev_buzzer_set_frequency(uint_fast8_t number, uint_fast16_t frequency)
{
    hal_buzzer_set_frequency(number, frequency);
}

// end of File
