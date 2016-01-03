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

#include "hal_uart.h"

bool hal_init_gcode_uart(uint_fast16_t rec_buf_size, uint_fast16_t send_buf_size)
{
    return true;
}

void hal_print_configuration_gcode_uart(void)
{

}

uint_fast8_t hal_get_gcode_uart_byte_at_offset(uint_fast16_t offset)
{
    return 0;
}

uint_fast16_t hal_get_available_bytes_gcode_uart(void)
{
    return 0;
}

void hal_forget_bytes_gcode_uart(uint_fast16_t how_many)
{

}

void hal_send_frame_gcode_uart(uint8_t * frame, uint_fast16_t length)
{

}

bool hal_send_frame_non_blocking_gcode_uart(uint8_t * frame, uint_fast16_t length)
{
    return false;
}

bool hal_init_debug_uart(uint_fast16_t rec_buf_size, uint_fast16_t send_buf_size)
{
    return true;
}

void hal_print_configuration_debug_uart(void)
{

}

uint_fast8_t hal_get_debug_uart_byte_at_offset(uint_fast16_t offset)
{
    return 0;
}

uint_fast16_t hal_get_available_bytes_debug_uart(void)
{
    return 0;
}

void hal_forget_bytes_debug_uart(uint_fast16_t how_many)
{

}

void hal_send_frame_debug_uart(uint8_t * frame, uint_fast16_t length)
{

}

bool hal_send_frame_non_blocking_debug_uart(uint8_t * frame, uint_fast16_t length)
{
    return true;
}
