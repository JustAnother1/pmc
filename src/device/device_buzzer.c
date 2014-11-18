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

void dev_buzzer_init(void)
{

}

uint_fast8_t dev_buzzer_get_count(void)
{
    return 0;
}

uint_fast8_t dev_buzzer_get_name(uint_fast8_t number, uint_fast8_t *position)
{
    return 0;
}

uint_fast8_t dev_buzzer_get_status(uint_fast8_t number)
{
    return DEVICE_STATUS_FAULT;
}

void dev_buzzer_set_pwm(uint_fast8_t number, uint_fast16_t pwm)
{

}

void dev_buzzer_set_frequency(uint_fast8_t number, uint_fast16_t frequency)
{

}

// end of File
