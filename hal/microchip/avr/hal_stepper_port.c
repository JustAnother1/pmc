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

#include "hal_stepper_port.h"

static uint32_t stepper_port_output = 0;

void hal_stepper_port_init(void)
{

}

void hal_stepper_set_Output(uint32_t value)
{
    stepper_port_output = value;
}

uint32_t hal_stepper_get_Output(void)
{
    return stepper_port_output;
}

