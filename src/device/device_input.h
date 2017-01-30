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

#ifndef DEVICE_INPUT_H_
#define DEVICE_INPUT_H_

#include <inttypes.h>

void dev_input_init(void);
uint_fast8_t dev_input_get_count(void);
uint_fast8_t dev_input_get_status(uint_fast8_t number);
uint_fast8_t dev_input_get_name(uint_fast8_t number, uint8_t *position);

#endif /* DEVICE_INPUT_H_ */
