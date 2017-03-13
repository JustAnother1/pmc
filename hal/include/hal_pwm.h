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

#ifndef HAL_INCLUDE_HAL_PWM_H_
#define HAL_INCLUDE_HAL_PWM_H_

#include <inttypes.h>
#include <stdbool.h>

// the first Devices (0, 1, 2..) are Heaters.
// later Devices (NUMBER_OF_HEATERS, NUMBER_OF_HEATERS +1, ..) are Fans

void hal_pwm_init(void);
uint_fast8_t hal_pwm_get_amount(void);
void hal_pwm_set_on_time(uint_fast8_t device, uint_fast16_t on_time);
uint_fast8_t hal_pwm_get_name(uint_fast8_t device, uint8_t *position, uint_fast8_t max_length);
#ifdef DEBUG_ACTIVE
bool hal_pwm_print_configuration(int pwmNumber, int lineNumber);
#endif
#endif /* HAL_INCLUDE_HAL_PWM_H_ */
