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

#ifndef DEVICE_OUTPUT_H_
#define DEVICE_OUTPUT_H_

#include <inttypes.h>

void dev_output_init(void);
uint_fast8_t dev_output_get_count(void);
uint_fast8_t dev_output_get_name(uint_fast8_t number, uint_fast8_t *position);
uint_fast8_t dev_output_get_status(uint_fast8_t number);
uint_fast8_t dev_output_get_switch_state(uint_fast8_t number);
void dev_output_set_switch_state(uint_fast8_t number, uint_fast8_t state);

#endif /* DEVICE_OUTPUT_H_ */
