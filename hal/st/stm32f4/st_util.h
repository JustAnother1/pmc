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

#ifndef HAL_ST_STM32F4_ST_UTIL_H_
#define HAL_ST_STM32F4_ST_UTIL_H_

#include <inttypes.h>

#include "st_gpio.h"

void print_gpio_configuration(GPIO_TypeDef * gpio);
void print_gpio_pin_configuration(GPIO_TypeDef * PortRegisters, int idx);
void uDelay(const uint32_t usec);
void mDelay(const uint32_t msec);

#endif /* HAL_ST_STM32F4_ST_UTIL_H_ */
