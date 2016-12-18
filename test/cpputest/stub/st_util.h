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

#ifndef TEST_CPPUTEST_STUB_ST_UTIL_H_
#define TEST_CPPUTEST_STUB_ST_UTIL_H_

uint_fast8_t copy_string(char * str, uint8_t *position);
void print_gpio_configuration(GPIO_TypeDef * gpio);
void print_gpio_pin_configuration(GPIO_TypeDef * PortRegisters, int idx);
void uDelay(const uint32_t usec);
void mDelay(const uint32_t msec);

#endif /* TEST_CPPUTEST_STUB_ST_UTIL_H_ */
