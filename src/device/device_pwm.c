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
#include "hal_pwm.h"
#include "hal_cfg.h"

void dev_pwm_init(void)
{
    hal_pwm_init();
}

uint_fast8_t dev_pwm_get_count(void)
{
    return hal_pwm_get_amount() - NUMBER_OF_HEATERS;
}

uint_fast8_t dev_pwm_get_name(uint_fast8_t number, uint8_t *position)
{
    return hal_pwm_get_name(number + NUMBER_OF_HEATERS, position);
}

uint_fast8_t dev_pwm_get_status(uint_fast8_t number)
{
    if(number + NUMBER_OF_HEATERS < hal_pwm_get_amount())
    {
        return DEVICE_STATUS_ACTIVE;
    }
    else
    {
        return DEVICE_STATUS_FAULT;
    }
}

void dev_pwm_set_pwm(uint_fast8_t number, uint_fast16_t pwm)
{
    hal_pwm_set_on_time(number + NUMBER_OF_HEATERS, pwm);
}

void dev_pwm_set_frequency(uint_fast8_t number, uint_fast16_t frequency)
{
    // TODO: Really ?
}

// end of File
