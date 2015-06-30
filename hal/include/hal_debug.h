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
void hal_debug_init(void);
void debug_msg(const char* format, ...);
void debug_line(const char* format, ...);
void debug_print32(uint32_t num);
#else
void hal_debug_init(void) {}
void debug_msg(const char* format, ...) {}
void debug_line(const char* format, ...) {}
void debug_print32(uint32_t num) {}
#endif

#endif /* HAL_INCLUDE_HAL_DEBUG_H_ */
