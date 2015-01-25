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

#include "util.h"

uint_fast8_t copy_string(char * str, uint8_t *position)
{
    uint_fast8_t num = 0;
    char c = str[num];
    while(c !=0)
    {
        *position = c;
        num++;
        c = str[num];
    }
    return num;
}
