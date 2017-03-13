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

#include "hal_pwm.h"

void hal_pwm_init(void)
{

}

uint_fast8_t hal_pwm_get_amount(void)
{
    return 0;
}

void hal_pwm_set_on_time(uint_fast8_t device, uint_fast16_t on_time)
{

}

uint_fast8_t hal_pwm_get_name(uint_fast8_t device, uint8_t *position, uint_fast8_t max_length)
{
    return 0;
}

#ifdef DEBUG_ACTIVE
bool hal_pwm_print_configuration(int pwmNumber, int lineNumber)
{
    return false;
}
#endif

