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

void hal_cpu_signal_error(void);
void hal_cpu_die(void);
void hal_cpu_do_software_reset(void);
void hal_cpu_ms_sleep(int ms);

#endif /* HAL_CPU_H_ */
