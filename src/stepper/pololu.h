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

#ifndef SRC_STEPPER_POLOLU_H_
#define SRC_STEPPER_POLOLU_H_

#include <stdint.h>

void pololu_init(void);
void pololu_enable_motor(uint_fast8_t stepper_number);
void pololu_disable_motor(uint_fast8_t stepper_number);
#ifdef DEBUG_ACTIVE
void pololu_print_status(void);
#endif

#endif /* SRC_STEPPER_POLOLU_H_ */
