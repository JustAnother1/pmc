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

#ifndef DEBUG_H_
#define DEBUG_H_

#include <inttypes.h>

#ifdef DEBUG_ACTIVE
void debug_init(void);
void debug_tick(void);
void debug_hex_buffer(uint8_t* buf, int length);
#endif

#endif /* DEBUG_H_ */
