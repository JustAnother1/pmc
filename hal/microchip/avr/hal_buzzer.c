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

#include "board_cfg.h"
#include "hal_cfg.h"
#include "hal_buzzer.h"

void hal_buzzer_init(void)
{
    // Output a Low(0) for a start
    BUZZER_PORT_1 &= ~BUZZER_PORT_1_MASK;
    // Set the Data Direction on the used pins
    BUZZER_DD_1 |= BUZZER_PORT_1_MASK;
    // Disable timers
    BUZZER_0_TCCRB = 0;
}

uint_fast8_t hal_buzzer_get_amount(void)
{
    return 1;
}

void hal_buzzer_set_frequency(uint_fast8_t device, uint_fast16_t frequency)
{
    switch(device)
    {
    case 0:
        if(0 == frequency)
        {
            // Disable timer
            BUZZER_0_TCCRB = 0;
        }
        else
        {
            BUZZER_0_OCRA = F_CPU/(16*frequency);
            BUZZER_0_TIMSK = 0x00; // no Interrupts !
            BUZZER_0_TCCRA = 0x54; // CTC toggle on match
            BUZZER_0_TCNT  = 0;
            BUZZER_0_TCCRB = 0x0A; // prescaler: /8 - CTC
        }
        break;

    default:
        // invalid device
        break;
    }
}

uint_fast8_t hal_buzzer_get_name(uint_fast8_t device, uint8_t *position, uint_fast8_t max_length)
{
    switch(device)
    {
    case  0: return strlcpy_P((char *)position, STR(BUZZER_0_NAME), max_length);
    default:
        return 0;
    }
}

