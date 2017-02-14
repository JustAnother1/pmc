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

#ifndef DEVICE_STEPPER_H_
#define DEVICE_STEPPER_H_

#include <inttypes.h>

void dev_stepper_init(void);
uint_fast8_t dev_stepper_get_count(void);
void dev_stepper_detectSteppers(void);
uint_fast8_t dev_stepper_get_name(uint_fast8_t number, uint8_t *position, uint_fast8_t max_length);
uint_fast8_t dev_stepper_get_status(uint_fast8_t number);
void dev_stepper_activate(uint_fast8_t on_off);
void dev_stepper_disable_all_motors(void);
void dev_stepper_enable_motor(uint_fast8_t stepper_number, uint_fast8_t on_off);
void dev_stepper_configure_axis_movement_rate(uint_fast8_t stepper_number, uint_fast32_t max_steps);
void dev_stepper_configure_mvmnt_unrun_avoid_para(uint_fast8_t stepper_number,
                                                  uint_fast32_t max_steps,
                                                  uint_fast32_t max_decelleration);

#endif /* DEVICE_STEPPER_H_ */
