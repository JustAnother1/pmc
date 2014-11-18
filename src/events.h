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

#ifndef EVENTS_H_
#define EVENTS_H_

#include <inttypes.h>
#include <stdbool.h>

void events_init(void);
void events_receive(uint_fast8_t last_id);
void events_get_number_format_ids(void);
void events_get_string_format(uint_fast16_t idx);
bool events_has_events(void);

#endif /* EVENTS_H_ */
