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

#include "uart.h"
#include "protocol.h"
#include "com.h"
#include "hal_uart.h"
#include "hal_cfg.h"

#define RECEIVE_BUFFER_SIZE  512
#define SEND_BUFFER_SIZE     100

static bool crc_is_valid(void);

static bool has_a_frame = false;
static uint_fast16_t checked_bytes = 0;

static uint_fast8_t length;

// TODO: Timeout if more than 20ms no byte has been received,
// then forget the already received bytes.

bool start_uart(void)
{
    return hal_init_gcode_uart(RECEIVE_BUFFER_SIZE, SEND_BUFFER_SIZE);
}

void uart_send_frame(uint8_t * frame, uint_fast16_t length)
{
    if(1 > length)
    {
        return;
    }
    // else :
    if(false == hal_send_frame_non_blocking_gcode_uart(frame, length))
    {
    	hal_send_frame_gcode_uart(frame, length);
    }
}

bool uart_has_next_frame(void)
{
    uint_fast16_t bytes_available;
    if(true == has_a_frame)
    {
        return true;
    }
    // else :
    // check the Frame:
    // Enough Bytes
    bytes_available = hal_get_available_bytes_gcode_uart();
    if(checked_bytes == bytes_available)
    {
        // we did a check on this number of Bytes and had no Frame
        return false;
    }
    else
    {
        // we will now check this amount of Bytes received.
        checked_bytes = bytes_available;
    }
    if(MIN_BYTES_HOST_FRAME > bytes_available)
    {
        return false;
    }
    // starts with a sync byte
    if(HOST_SYNC_REQUEST != hal_get_gcode_uart_byte_at_offset(0))
    {
    	hal_forget_bytes_gcode_uart(1);
        checked_bytes = 0;
        return false;
    }
    // length OK and also enough bytes for Parameter
    length = hal_get_gcode_uart_byte_at_offset(1);
    if(length + 3 > bytes_available)
    {
        return false;
    }
    // CRC valid ?
    if(false == crc_is_valid())
    {
        uart_forget_frame();
        return false;
    }
    has_a_frame = true;
    return has_a_frame;
}

uint_fast8_t uart_get_order(void)
{
    return hal_get_gcode_uart_byte_at_offset(REQUEST_FRAME_POS_OF_ORDER_CODE);
}

uint_fast8_t uart_get_parameter_length(void)
{
    return length - REQUEST_NUMBER_OF_NON_PARAMETER_BYTES_IN_LENGTH;
}

uint_fast8_t uart_get_control(void)
{
    return hal_get_gcode_uart_byte_at_offset(REQUEST_FRAME_POS_OF_CONTROL);
}

uint_fast8_t uart_get_parameter_byte(uint_fast8_t index)
{
    return hal_get_gcode_uart_byte_at_offset(REQUEST_FRAME_START_OF_PARAMETER + index);
}

void uart_forget_frame(void)
{
	hal_forget_bytes_gcode_uart(MIN_BYTES_HOST_FRAME + uart_get_parameter_length());
    checked_bytes = 0;
    has_a_frame = false;
}

static bool crc_is_valid(void)
{
    // CRC valid ?
    uint_fast8_t received_crc = hal_get_gcode_uart_byte_at_offset(length + 2);
    uint_fast8_t claculated_crc = com_crc_a_byte(length, 0);
    uint_fast16_t off;
    for(off = 0; off < length; off++)
    {
        claculated_crc = com_crc_a_byte(hal_get_gcode_uart_byte_at_offset(off + 2), claculated_crc);
    }

    if(claculated_crc != received_crc)
    {
        com_send_bad_crc(received_crc, claculated_crc);
        return false;
    }
    else
    {
        return true;
    }
}

// end of File
