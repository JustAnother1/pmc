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

#include "hal_cpu.h"
#include "hal_time.h"
#include "hal_common.h"
#include "hal_led.h"

static void hal_time_ISR(void);

static volatile uint32_t now = 0;

void hal_time_init(void)
{
    hal_cpu_add_ms_tick_function(&hal_time_ISR);
}

static void hal_time_ISR(void)
{
    now++;
}

uint32_t hal_time_get_ms_tick(void)
{
    return now;
}

void hal_time_ms_sleep(uint_fast32_t ms)
{
    uint32_t tickend = hal_time_get_ms_tick() + ms;
    uint32_t curtick = hal_time_get_ms_tick();
    while(curtick < tickend)
    {
        curtick = hal_time_get_ms_tick();
    }
}

bool hal_time_start_timer(uint_fast8_t device, uint16_t reload_value, TimerFkt function)
{
    // TODO
    return false;
}
void hal_time_stop_timer(uint_fast8_t device)
{

}

bool hal_time_set_timer_reload(uint_fast8_t device, uint16_t reload_value)
{
    return false;
}

