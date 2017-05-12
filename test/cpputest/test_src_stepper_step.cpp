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

#include "CppUTest/TestHarness.h"
#include "CppUTest/TestOutput.h"
#include "CppUTest/TestTestingFixture.h"
#include "CppUTest/PlatformSpecificFunctions.h"

extern "C"
{
#include "step.h"

extern uint_fast8_t direction_for_move;  // bitmap
extern volatile uint_fast16_t active_axes_map;
extern uint_fast8_t available_steppers;
extern bool enabled[MAX_NUMBER];
extern bool is_a_homing_move;
extern volatile bool busy;
}

TEST_GROUP(SrcStepperStepTestGroup)
{

};

TEST(SrcStepperStepTestGroup, step_print_state)
{
    step_print_state();
}

TEST(SrcStepperStepTestGroup, step_is_moving_towards)
{
    direction_for_move = 0;
    active_axes_map = 0;
    CHECK_FALSE(step_is_moving_towards(0, false));
    CHECK_FALSE(step_is_moving_towards(0, true));
    active_axes_map = 1;
    direction_for_move = 0;
    CHECK_FALSE(step_is_moving_towards(0, true));
    CHECK(step_is_moving_towards(0, false));
    active_axes_map = 1;
    direction_for_move = 1;
    CHECK(step_is_moving_towards(0, true));
    CHECK_FALSE(step_is_moving_towards(0, false));
}

TEST(SrcStepperStepTestGroup, step_enable_motor)
{
    for(int i = 0; i < MAX_NUMBER; i++)
    {
        enabled[i] = false;
    }
    available_steppers = 5;
    step_enable_motor(0, 0);
    step_enable_motor(6, 0);
    CHECK_FALSE(enabled[0]);
    CHECK_FALSE(enabled[6]);
    enabled[2] = true;
    step_enable_motor(2, 0);
    CHECK_FALSE(enabled[2]);
    step_enable_motor(2, 1);
    CHECK(enabled[2]);
    step_enable_motor(2, 1);
    CHECK(enabled[2]);
}

TEST(SrcStepperStepTestGroup, step_disable_all_motors)
{
    for(int i = 0; i < MAX_NUMBER; i++)
    {
        enabled[i] = true;
    }
    available_steppers = 5;
    step_disable_all_motors();
    CHECK_FALSE(enabled[0]);
    CHECK_FALSE(enabled[1]);
    CHECK_FALSE(enabled[2]);
    CHECK_FALSE(enabled[3]);
    CHECK_FALSE(enabled[4]);
    CHECK(enabled[5]);
    CHECK(enabled[6]);
    CHECK(enabled[7]);
}

TEST(SrcStepperStepTestGroup, step_is_homing)
{
    is_a_homing_move = true;
    CHECK(step_is_homing());
}

TEST(SrcStepperStepTestGroup, step_is_busy)
{
    busy = true;
    CHECK(step_is_busy());
}

TEST(SrcStepperStepTestGroup, step_add_delay)
{
    busy = true;
    CHECK_FALSE(step_add_delay(10));
    busy = false;
    CHECK(step_add_delay(10));
    CHECK(busy);
}
