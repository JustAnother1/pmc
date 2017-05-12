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

extern "C"
{

#include <inttypes.h>
#include <stdbool.h>

typedef void (*TimerFkt)(void);

void hal_time_init(void)
{

}

void hal_time_ms_sleep(uint_fast32_t ms)
{

}

bool hal_time_start_timer(uint_fast8_t device,  // Timer NUmber
                          uint32_t clock,       // Frequency that the timer runs on
                          uint_fast16_t reload_value,
                          TimerFkt function)
{
    return false;
}

bool hal_time_set_timer_reload(uint_fast8_t device, uint16_t reload_value)
{
    return false;
}

void hal_time_stop_timer(uint_fast8_t device)
{

}

bool hal_time_enable_pwm_for(uint_fast8_t device)
{
    return false;
}

bool hal_time_set_PWM_for(uint_fast8_t device, uint_fast8_t channel, uint16_t pwm_value)
{
    return false;
}

bool hal_time_stop_pwm_for(uint_fast8_t device, uint_fast8_t channel)
{
    return false;
}

bool hal_time_print_Configuration(int timerNumber, int lineNumber)
{
    return false;
}

}

