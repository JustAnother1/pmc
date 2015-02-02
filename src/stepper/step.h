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

#ifndef STEP_H_
#define STEP_H_

#include <inttypes.h>
#include <stdbool.h>

void step_init(void);
uint_fast8_t step_detect_number_of_steppers(void);
void step_configure_steppers(uint_fast8_t num_steppers);
// returns TRUE wile execution a Move.
// returns FALSE when ready for the next move
bool step_is_busy(void);

bool step_add_basic_linear_move(uint_fast8_t *move_data);
bool step_add_delay(uint_fast8_t msb,uint_fast8_t lsb);
void step_request_tag(void);
bool step_has_reached_tag(void);

#endif /* STEP_H_ */
