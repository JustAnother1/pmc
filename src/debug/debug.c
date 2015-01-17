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

#include <stdio.h>
#include <stdarg.h>
#include "hal_cfg.h"
#include "hal_uart.h"
#include "hal_time.h"
#include "error.h"
#include "debug.h"

#define MSG_BUFFER_LENGTH 100
uint8_t buffer [MSG_BUFFER_LENGTH];

// ticks per millisecond
uint_fast32_t tick_cnt;
uint_fast32_t tick_value;
uint_fast32_t tick_max;
uint_fast32_t tick_min;

void debug_init(void)
{
    int num;
    // Tick counting
    tick_cnt = 0;
    tick_value = 0;
    tick_max = 0;
    tick_min = 42424242;
    // UART
    bool res = false;
    res = hal_uart_init(DEBUG_UART);
    if(false == res)
    {
        error_signal_error_and_die();
    }
    num = snprintf((char *)buffer, MSG_BUFFER_LENGTH, "Debug Console PMC\n (db)");
    hal_uart_send_frame(DEBUG_UART, &buffer[0], num);
}

void debug_tick(void)
{
    // do some statistics / supervision tasks
    // check if we received an order
    uint_fast32_t time =  hal_time_get_ms_tick();
    if(tick_value == time)
    {
        // another tick in this ms
        tick_cnt++;
    }
    else
    {
        if(0 != tick_value)
        {
            // record the statistics
            if(tick_cnt > tick_max)
            {
                tick_max = tick_cnt;
                debug_msg("New tick max: %d", tick_max);
            }
            if(tick_cnt < tick_min)
            {
                tick_min = tick_cnt;
                debug_msg("New tick min: %d", tick_min);
            }
        }
        tick_value = time;
        tick_cnt = 0;
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



