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

#include <stdint.h>

int atoi(const char *c)
{
    int previous_result = 0;
    int result = 0;
    int multiplier = 1;

    if (c && *c == '-')
    {
        multiplier = -1;
        c++;
    }
    else
    {
        multiplier = 1;
    }

    while (*c)
    {
        if (*c < '0' || *c > '9')
        {
            return result * multiplier;
        }
        result *= 10;
        if (result < previous_result)
        {
            // number overflowed - return INT_MIN
            return(INT32_MIN);
        }
        else
        {
            previous_result = result;
        }

        result += *c - '0';

        if (result < previous_result)
        {
            // number overflowed - return MIN_INT
            return(INT32_MIN);
        }
        else
        {
            previous_result = result;
        }
        c++;
    }
    return(result * multiplier);
}
