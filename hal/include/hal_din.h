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

#ifndef HAL_INCLUDE_HAL_DIN_H_
#define HAL_INCLUDE_HAL_DIN_H_

#include <inttypes.h>
#include <stdbool.h>

typedef void (*din_func)(bool high, uint_fast8_t stepper, uint_fast8_t switch_number);


void hal_din_init(void);
uint_fast8_t hal_din_get_amount(void);
uint_fast8_t hal_din_get_name(uint_fast8_t device, uint8_t *position, uint_fast8_t max_length);
uint_fast8_t hal_din_get_switch_state(uint_fast8_t device);
void hal_din_subscribe_to_events(uint_fast8_t switch_number, uint_fast8_t stepper_number, din_func handle_func);
#ifdef DEBUG_ACTIVE
void hal_din_print_PinConfiguration(uint_fast8_t port, int idx);
#endif

#ifdef POLL_END_STOPS
void hal_din_poll(void);
#endif

#endif /* HAL_INCLUDE_HAL_DIN_H_ */
