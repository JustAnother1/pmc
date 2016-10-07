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

void hal_debug_init(void)
{
}

void debug_msg(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf(format, args );
    va_end(args);
}

void debug_line(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf(format, args );
    va_end(args);
    printf("\r\n");
}

void debug_print32(uint32_t num)
{
    printf("%08x", num);
}

void debug_printChar(char c)
{
    printf("%c", c);
}

void curTest(void)
{

}

