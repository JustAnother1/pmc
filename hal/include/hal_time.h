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

#ifndef HAL_TIME_H_
#define HAL_TIME_H_

#include <inttypes.h>
#include <stdbool.h>

typedef void (*TimerFkt)(void);

void hal_time_init(void);
uint32_t hal_time_get_ms_tick(void);
void hal_time_ms_sleep(uint_fast32_t ms);
bool hal_time_start_timer(uint_fast8_t device, uint32_t clock, uint_fast16_t reload_value, TimerFkt function);
bool hal_time_set_timer_reload(uint_fast8_t device, uint16_t reload_value);
void hal_time_stop_timer(uint_fast8_t device);

#endif /* HAL_TIME_H_ */
