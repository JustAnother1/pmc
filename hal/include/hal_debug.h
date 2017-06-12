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

#ifndef HAL_INCLUDE_HAL_DEBUG_H_
#define HAL_INCLUDE_HAL_DEBUG_H_

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

typedef void (*putcf) (void*, char);

#ifdef DEBUG_ACTIVE

#include "lib/printf.h"
#include "hal_cfg.h"

#define debug_msg(...)  tfp_printf(__VA_ARGS__)
#define debug_line(...) tfp_printf(__VA_ARGS__); tfp_printf(STR("\r\n"))

void hal_debug_init(void);
bool hal_debug_is_send_buffer_empty(void);
putcf getDebugOutput(void);
#else
#define debug_msg(...)
#define debug_line(...)
#define hal_debug_is_send_buffer_empty()  (true)
#define getDebugOutput() (NULL)
#endif

#endif /* HAL_INCLUDE_HAL_DEBUG_H_ */
