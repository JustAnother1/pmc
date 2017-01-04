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

#ifdef DEBUG_ACTIVE

#include "printf.h"

#define debug_msg(...)  tfp_printf(__VA_ARGS__)
#define debug_line(...) tfp_printf(__VA_ARGS__); tfp_printf("\r\n")

void hal_debug_init(void);
#else
#define debug_msg(...)
#define debug_line(...)
#endif

#endif /* HAL_INCLUDE_HAL_DEBUG_H_ */
