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

#include "hal_buzzer.h"
#include "board_cfg.h"
#include "util.h"

void hal_buzzer_init(void)
{

}

uint_fast8_t hal_buzzer_get_amount(void)
{
    return BUZZER_NUM_PINS;
}

void hal_buzzer_set_frequency(uint_fast8_t device, uint_fast16_t frequency)
{

}

uint_fast8_t hal_buzzer_get_name(uint_fast8_t number, uint8_t *position)
{
    if(number < BUZZER_NUM_PINS)
    {
        switch(number)
        {
        case  0: return copy_string(BUZZER_0_NAME, position);
        default:
            return 0;
        }
    }
    else
    {
        return 0;
    }
}

