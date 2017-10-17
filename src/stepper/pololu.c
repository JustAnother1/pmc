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

#include "pololu.h"
#include "hal_cfg.h"
#include "hal_debug.h"
#include "hal_stepper_port.h"
#include "step.h"
#include "board_cfg.h"


void pololu_init(void)
{

}

void pololu_enable_motor(uint_fast8_t stepper_number)
{
    if(stepper_number < NUMBER_OF_STEPPERS)
    {
        hal_setEnable(stepper_number, true);
    }
    // else this board does not have that stepper
}

void pololu_disable_motor(uint_fast8_t stepper_number)
{
    if(stepper_number < NUMBER_OF_STEPPERS)
    {
        hal_setEnable(stepper_number, false);
    }
    // else this board does not have that stepper
}

#ifdef DEBUG_ACTIVE
void pololu_print_status()
{
    step_print_state();
}
#endif

