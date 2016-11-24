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

#ifndef HAL_CPU_H_
#define HAL_CPU_H_

#include <inttypes.h>

typedef void (*msTickFkt)(void);

void hal_cpu_init_hal(void);
void hal_cpu_add_ms_tick_function(msTickFkt additional_function);
void hal_cpu_add_ms_tick_function_cycle(msTickFkt additional_function, int everyMs);
void hal_cpu_die(void);
void hal_cpu_do_software_reset(void);
void hal_cpu_tick(void);
uint32_t hal_cpu_get_ms_tick(void);
void hal_cpu_check_Reset_Reason(void);

#endif /* HAL_CPU_H_ */
