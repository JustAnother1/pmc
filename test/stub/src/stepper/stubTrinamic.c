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

#include "trinamic.h"

void trinamic_init(void)
{

}

bool trinamic_change_setting(uint8_t* setting)
{
    return true;
}

uint_fast8_t trinamic_detect_number_of_steppers(void)
{
    return 0;
}

void trinamic_configure_steppers(uint_fast8_t num_steppers)
{

}

void trinamic_enable_stepper(uint_fast8_t stepper_num)
{

}

void trinamic_disable_stepper(uint_fast8_t stepper_num)
{

}

#ifdef USE_STEP_DIR

void trinamic_print_stepper_status(void)
{

}

#else

void trinamic_make_step_using_SPI(uint_fast8_t stepper_num, bool direction_is_increasing)
{

}

#endif

