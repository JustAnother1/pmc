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

#ifndef HAL_UART_H_
#define HAL_UART_H_

#include <inttypes.h>
#include <stdbool.h>
#include "hal_cfg.h"

bool hal_uart_init(uint_fast8_t device, uint_fast16_t rec_buf_size, uint_fast16_t send_buf_size);
void hal_uart_print_configuration(uint_fast8_t device);
uint_fast8_t hal_uart_get_byte_at_offset(uint_fast8_t device, uint_fast16_t offset);
uint_fast16_t hal_uart_get_available_bytes(uint_fast8_t device);
void hal_uart_forget_bytes(uint_fast8_t device, uint_fast16_t how_many);
void hal_uart_send_frame(uint_fast8_t device, uint8_t * frame, uint_fast16_t length);
void hal_uart_send_frame_non_blocking(uint_fast8_t device, uint8_t * frame, uint_fast16_t length);
#endif /* HAL_UART_H_ */
