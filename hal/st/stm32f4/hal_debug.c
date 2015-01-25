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
#include "hal_cfg.h"
#include "hal_uart.h"
#include "hal_led.h"
#include "hal_cpu.h"

#define MSG_BUFFER_LENGTH 100
uint8_t buffer[MSG_BUFFER_LENGTH];

void hal_debug_init(void)
{
    // UART
    bool res = false;
    res = hal_uart_init(DEBUG_UART);
    if(false == res)
    {
        hal_led_set_error_led(true);
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
    hal_uart_send_frame(DEBUG_UART, &buffer[0], nwritten);
}


