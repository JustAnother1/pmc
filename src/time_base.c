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

#include "time_base.h"
#include "hal_time.h"

#include <inttypes.h>

static uint32_t now = 0;

// start the milli second counter -  Timer 5
void time_base_init(void)
{
    hal_time_init();
}

void time_base_ISR(void)
{
    now++;
}

uint32_t time_base_get_tick(void)
{
    return now;
}

// end of File
