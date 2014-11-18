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

#include <inttypes.h>
#include <stdbool.h>
#include "events.h"

static bool has_uncollected_events;

void events_init(void)
{
    has_uncollected_events = false;
}

void events_receive(uint_fast8_t last_id)
{

}

void events_get_number_format_ids(void)
{

}

void events_get_string_format(uint_fast16_t idx)
{

}

// returns TRUE if a Event has occurred and is ready for collection by the host.
bool events_has_events(void)
{
    return has_uncollected_events;
}

// end of File
