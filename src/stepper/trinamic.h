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

#ifndef SRC_STEPPER_TRINAMIC_H_
#define SRC_STEPPER_TRINAMIC_H_

#include <inttypes.h>
#include <stdbool.h>

uint_fast8_t trinamic_detect_number_of_steppers(void);
void trinamic_configure_steppers(uint_fast8_t num_steppers);
void trinamic_enable_stepper(uint_fast8_t stepper_num);
void trinamic_disable_stepper(uint_fast8_t stepper_num);
#ifdef USE_STEP_DIR
#else
void trinamic_make_step_using_SPI(uint_fast8_t stepper_num, bool direction_is_increasing);
#endif

#endif /* SRC_STEPPER_TRINAMIC_H_ */