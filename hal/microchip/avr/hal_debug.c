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
#include <stdarg.h>
#include "hal_cfg.h"
#include "hal_uart.h"
#include "hal_cpu.h"
#include "hal_led.h"
#include "lib/printf.h"

static void debug_putc( void* p, char c);

void hal_debug_init(void)
{
    hal_init_debug_uart();
    init_printf(NULL, debug_putc);
}

putcf getDebugOutput(void)
{
    return debug_putc;
}

static void debug_putc(void* p, char c)
{
    (void)p;
    // TODO hal_send_frame_debug_uart((uint8_t *)&c, 1);

    if(false == hal_send_frame_non_blocking_debug_uart((uint8_t *)&c, 1))
    {
        hal_set_error_led(true);
        hal_cpu_report_issue(5);
    }
}

bool hal_debug_is_send_buffer_empty(void)
{
    return hal_is_debug_uart_send_buffer_empty();
}

