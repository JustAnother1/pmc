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

#ifndef SRC_ENDSTOPHANDLING_H_
#define SRC_ENDSTOPHANDLING_H_

#include <inttypes.h>
#include <stdbool.h>

void endStopHandling_init(void);
bool dev_input_enable(uint_fast8_t switch_number, uint_fast8_t enable);
bool dev_input_is_enabled(uint_fast8_t switch_number);
bool dev_stepper_configure_end_stops(uint_fast8_t StepperNumber, uint_fast8_t switch_number, uint_fast8_t min_Max);
uint_fast8_t dev_input_get_switch_state(uint_fast8_t number);

#endif /* SRC_ENDSTOPHANDLING_H_ */
