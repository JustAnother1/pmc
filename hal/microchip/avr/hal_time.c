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

#include <stdio.h>
#include <stdlib.h>
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#include "hal_time.h"
#include "hal_pwm.h"
#include "board_cfg.h"
#include "hal_cfg.h"


void hal_time_init(void)
{

}

void hal_time_ms_sleep(uint_fast32_t ms)
{
     _delay_ms(ms);
}

bool hal_time_start_timer(uint_fast8_t device, uint32_t clock, uint_fast16_t reload_value, TimerFkt function)
{
    (void) clock;
    (void) function;
    if(STEP_TIMER == device)
    {
        STEP_TIMER_OCRA = reload_value;
        STEP_TIMER_TCCRA = (STEP_TIMER_TCCRA | STEP_TIMER_TCCRA_1) &~STEP_TIMER_TCCRA_0;
        STEP_TIMER_TCCRB = (STEP_TIMER_TCCRB | STEP_TIMER_TCCRB_1) &~STEP_TIMER_TCCRB_0;
        STEP_TIMER_TIMSK = STEP_TIMER_TIMSK_VALUE;
        return true;
    }
    return false;
}
void hal_time_stop_timer(uint_fast8_t device)
{
    if(STEP_TIMER == device)
    {
        STEP_TIMER_TCCRA = 0;
        STEP_TIMER_TCCRB = 0; // Stop Timer
        STEP_TIMER_TIMSK = 0; // disable all interrupts
    }
}

bool hal_time_set_timer_reload(uint_fast8_t device, uint16_t reload_value)
{
    if(STEP_TIMER == device)
    {
        STEP_TIMER_OCRA = reload_value;
        return true;
    }
    return false;
}

#ifdef DEBUG_ACTIVE
bool hal_time_print_Configuration(int timerNumber, int lineNumber)
{
    return hal_pwm_print_configuration(timerNumber, lineNumber);
}
#endif

