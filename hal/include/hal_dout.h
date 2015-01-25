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

#ifndef HAL_INCLUDE_HAL_DOUT_H_
#define HAL_INCLUDE_HAL_DOUT_H_

#include <inttypes.h>

void hal_dout_init(void);
uint_fast8_t hal_dout_get_amount(void);
void hal_dout_set_pin_high(uint_fast8_t number);
void hal_dout_set_pin_low(uint_fast8_t number);
uint_fast8_t hal_dout_get_name(uint_fast8_t number, uint8_t *position);


#endif /* HAL_INCLUDE_HAL_DOUT_H_ */
