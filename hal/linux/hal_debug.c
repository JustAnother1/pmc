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
#include "lib/printf.h"

static void debug_putc( void* p, char c);

void hal_debug_init(void)
{
    init_printf(NULL, debug_putc);
}

putcf getDebugOutput(void)
{
    return debug_putc;
}

static void debug_putc(void* p, char c)
{
    putc(c, stdout);
}

bool hal_debug_is_send_buffer_empty(void)
{
    return true;
}
