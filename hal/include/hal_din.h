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

//End Stops:
#define MAX_END           1
#define MIN_END           0
#define MAX_NUM_END_STOPS 6
#define END_STOP_ENABLED  1
#define END_STOP_DISABLED 0
#define DIN_ERROR         2
#define DIN_HIGH          1
#define DIN_LOW           0

typedef void (*din_func)(bool high, uint_fast8_t stepper, uint_fast8_t switch_number);


void hal_din_init(void);
uint_fast8_t hal_din_get_amount(void);
uint_fast8_t hal_din_get_name(uint_fast8_t device, uint8_t *position, uint_fast8_t max_length);
/** reports the logic level read at the input pin of the switch input.
 * @param device switch number
 * @return Returns DIN_HIGH for high level at pin and DIN_LOW for low level at pin.
 *         Invalid switches are reported as being DIN_ERROR.
 */
uint_fast8_t hal_din_get_switch_state(uint_fast8_t device);
void hal_din_subscribe_to_events(uint_fast8_t switch_number, uint_fast8_t stepper_number, din_func handle_func);
#ifdef DEBUG_ACTIVE
void hal_din_print_PinConfiguration(uint_fast8_t port, int idx);
#endif

#ifdef POLL_END_STOPS
void hal_din_poll(void);
void hal_din_tick(void);
#endif

#endif /* HAL_INCLUDE_HAL_DIN_H_ */
