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

#ifndef UART_H_
#define UART_H_

#include <inttypes.h>
#include <stdbool.h>

bool start_uart(void);
bool uart_has_next_frame(void);
uint_fast8_t uart_get_order(void);
uint_fast8_t uart_get_parameter_byte(uint_fast8_t index);
uint_fast8_t uart_get_parameter_length(void);
uint_fast8_t uart_get_control(void);
void uart_send_frame(uint_fast8_t * frame, uint_fast16_t length);
void uart_forget_frame(void);

#endif /* UART_H_ */
