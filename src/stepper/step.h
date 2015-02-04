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

// max allowed number of steppers
#define MAX_NUMBER 8

void step_init(void);
// returns TRUE wile execution a Move.
// returns FALSE when ready for the next move
bool step_is_busy(void);

bool step_add_basic_linear_move(uint_fast8_t *move_data);
bool step_add_delay(uint_fast8_t msb,uint_fast8_t lsb);
void step_request_tag(void);
bool step_has_reached_tag(void);
void step_disable_all_motors(void);
void step_enable_motor(uint_fast8_t stepper_number, uint_fast8_t on_off);

#endif /* STEP_H_ */
