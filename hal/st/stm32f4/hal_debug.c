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

#include "hal_debug.h"
#include <stdbool.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdarg.h>
#include "hal_uart.h"
#include "hal_led.h"
#include "hal_cpu.h"

#define RECEIVE_BUFFER_SIZE  50
#define SEND_BUFFER_SIZE     2048

#define MSG_BUFFER_LENGTH 100
uint8_t buffer[MSG_BUFFER_LENGTH];

void hal_debug_init(void)
{
    // TODO create alternative implementation that uses core_cm4.h:ITM_SendChar() / ITM_ReceiveChar()
    // UART
    bool res = false;
    res = hal_init_debug_uart(RECEIVE_BUFFER_SIZE, SEND_BUFFER_SIZE);
    if(false == res)
    {
        hal_set_error_led(true);
        hal_cpu_die();
    }
}

void debug_msg(const char* format, ...)
{
    int nwritten = 0;
    va_list args;
    va_start(args, format);
    nwritten = vsnprintf((char *)buffer, MSG_BUFFER_LENGTH, format, args );
    va_end(args);
    if(false == hal_send_frame_non_blocking_debug_uart(&buffer[0], nwritten))
    {
        hal_set_error_led(true);
    }
}

void debug_line(const char* format, ...)
{
    int nwritten = 0;
    va_list args;
    va_start(args, format);
    nwritten = vsnprintf((char *)buffer, MSG_BUFFER_LENGTH, format, args );
    va_end(args);
    if(nwritten < MSG_BUFFER_LENGTH)
    {
        buffer[nwritten] = '\r';
        buffer[nwritten + 1] = '\n';
        nwritten += 2;
    }
    else
    {
        buffer[MSG_BUFFER_LENGTH -4] = '.';
        buffer[MSG_BUFFER_LENGTH -3] = '.';
        buffer[MSG_BUFFER_LENGTH -2] = '\r';
        buffer[MSG_BUFFER_LENGTH -1] = '\n';
        nwritten = MSG_BUFFER_LENGTH;
    }
    if(false == hal_send_frame_non_blocking_debug_uart(&buffer[0], nwritten))
    {
        hal_set_error_led(true);
    }
}

void debug_print32(uint32_t num)
{
    buffer[0] = '*';
    buffer[1] = (num>>24)  &0xff;
    buffer[2] = (num>>16) & 0xff;
    buffer[3] = (num>>8) & 0xff;
    buffer[4] = num & 0xff;
    buffer[5] = '+';
    if(false == hal_send_frame_non_blocking_debug_uart(&buffer[0], 6))
    {
        hal_set_error_led(true);
    }
}

void debug_printChar(char c)
{
    if(false == hal_send_frame_non_blocking_debug_uart((uint8_t *)&c, 1))
    {
        hal_set_error_led(true);
    }
}

