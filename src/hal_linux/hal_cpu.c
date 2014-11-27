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

#include <unistd.h>
#include <stdlib.h>

#include "hal_cpu.h"

void hal_cpu_init_hal(void)
{

}

void hal_cpu_die(void)
{
    exit(1);
}

void hal_cpu_do_software_reset(void)
{

}

void hal_cpu_ms_sleep(int ms)
{
    usleep(1000);
}
