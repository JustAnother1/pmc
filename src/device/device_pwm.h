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

#ifndef DEVICE_PWM_H_
#define DEVICE_PWM_H_

#include <inttypes.h>

void dev_pwm_init(void);
uint_fast8_t dev_pwm_get_count(void);
uint_fast8_t dev_pwm_get_name(uint_fast8_t number, uint_fast8_t *position);
uint_fast8_t dev_pwm_get_status(uint_fast8_t number);
void dev_pwm_set_pwm(uint_fast8_t number, uint_fast16_t pwm);
void dev_pwm_set_frequency(uint_fast8_t number, uint_fast16_t frequency);

#endif /* DEVICE_PWM_H_ */
