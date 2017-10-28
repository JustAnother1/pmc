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

#ifndef HAL_INCLUDE_HAL_STEPPER_PORT_H_
#define HAL_INCLUDE_HAL_STEPPER_PORT_H_

#include <inttypes.h>
#include <stdbool.h>

void hal_stepper_port_init(void);
void hal_setEnable(uint_fast8_t stepper_number, bool on);
void hal_stepper_set_Output(uint32_t value);
uint32_t hal_stepper_get_Output(void);

#endif /* HAL_INCLUDE_HAL_STEPPER_PORT_H_ */
