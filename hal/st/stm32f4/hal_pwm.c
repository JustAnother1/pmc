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
#include "board_cfg.h"
#include "util.h"

void hal_pwm_init(void)
{

}

uint_fast8_t hal_pwm_get_amount(void)
{
    return PWM_NUM_PINS;
}

void hal_pwm_set_on_time(uint_fast8_t device, uint_fast16_t on_time)
{

}

uint_fast8_t hal_pwm_get_name(uint_fast8_t device, uint8_t *position)
{
    if(device < PWM_NUM_PINS)
    {
        switch(device)
        {
        case  0: return copy_string(PWM_0_NAME, position);
        case  1: return copy_string(PWM_1_NAME, position);
        case  2: return copy_string(PWM_2_NAME, position);
        case  3: return copy_string(PWM_3_NAME, position);
        case  4: return copy_string(PWM_4_NAME, position);
        case  5: return copy_string(PWM_5_NAME, position);
        case  6: return copy_string(PWM_6_NAME, position);
        case  7: return copy_string(PWM_7_NAME, position);
        case  8: return copy_string(PWM_8_NAME, position);
        case  9: return copy_string(PWM_9_NAME, position);
        case 10: return copy_string(PWM_10_NAME, position);
        case 11: return copy_string(PWM_11_NAME, position);
        case 12: return copy_string(PWM_12_NAME, position);
        case 13: return copy_string(PWM_13_NAME, position);
        default:
            return 0;
        }
    }
    else
    {
        return 0;
    }
}

