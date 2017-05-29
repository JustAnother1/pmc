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
#include "hal_cfg.h"
#include "board_cfg.h"
#include "step.h"

static uint32_t stepper_port_output;

void hal_stepper_port_init(void)
{
    stepper_port_output = 0;
}

void hal_stepper_set_Output(uint32_t value)
{
    // Direction
    if((stepper_port_output & 0xff00) != (value & 0xff00))
    {
        // direction change
#if NUMBER_OF_STEPPERS > 0
        if((stepper_port_output & 0x0100) != (value & 0x0100))
        {
            // direction change on this axis
            if(0 == (value & 0x0100))
            {
                STEPPER_0_DIR_PORT &= ~STEPPER_0_DIR_MASK;
            }
            else
            {
                STEPPER_0_DIR_PORT |=  STEPPER_0_DIR_MASK;
            }
        }
        // else no direction change on this stepper
#endif

#if NUMBER_OF_STEPPERS > 1
        if((stepper_port_output & 0x0200) != (value & 0x0200))
        {
            // direction change on this axis
            if(0 == (value & 0x0200))
            {
                STEPPER_1_DIR_PORT &= ~STEPPER_1_DIR_MASK;
            }
            else
            {
                STEPPER_1_DIR_PORT |=  STEPPER_1_DIR_MASK;
            }
        }
        // else no direction change on this stepper
#endif

#if NUMBER_OF_STEPPERS > 2
        if((stepper_port_output & 0x0400) != (value & 0x0400))
        {
            // direction change on this axis
            if(0 == (value & 0x0400))
            {
                STEPPER_2_DIR_PORT &= ~STEPPER_2_DIR_MASK;
            }
            else
            {
                STEPPER_2_DIR_PORT |=  STEPPER_2_DIR_MASK;
            }
        }
        // else no direction change on this stepper
#endif

#if NUMBER_OF_STEPPERS > 3
        if((stepper_port_output & 0x0800) != (value & 0x0800))
        {
            // direction change on this axis
            if(0 == (value & 0x0800))
            {
                STEPPER_3_DIR_PORT &= ~STEPPER_3_DIR_MASK;
            }
            else
            {
                STEPPER_3_DIR_PORT |=  STEPPER_3_DIR_MASK;
            }
        }
        // else no direction change on this stepper
#endif

#if NUMBER_OF_STEPPERS > 4
        if((stepper_port_output & 0x1000) != (value & 0x1000))
        {
            // direction change on this axis
            if(0 == (value & 0x1000))
            {
                STEPPER_4_DIR_PORT &= ~STEPPER_4_DIR_MASK;
            }
            else
            {
                STEPPER_4_DIR_PORT |=  STEPPER_4_DIR_MASK;
            }
        }
        // else no direction change on this stepper
#endif

#if NUMBER_OF_STEPPERS > 5
        if((stepper_port_output & 0x2000) != (value & 0x2000))
        {
            // direction change on this axis
            if(0 == (value & 0x2000))
            {
                STEPPER_5_DIR_PORT &= ~STEPPER_5_DIR_MASK;
            }
            else
            {
                STEPPER_5_DIR_PORT |=  STEPPER_5_DIR_MASK;
            }
        }
        // else no direction change on this stepper
#endif

#if NUMBER_OF_STEPPERS > 6
        if((stepper_port_output & 0x4000) != (value & 0x4000))
        {
            // direction change on this axis
            if(0 == (value & 0x4000))
            {
                STEPPER_6_DIR_PORT &= ~STEPPER_6_DIR_MASK;
            }
            else
            {
                STEPPER_6_DIR_PORT |=  STEPPER_6_DIR_MASK;
            }
        }
        // else no direction change on this stepper
#endif

#if NUMBER_OF_STEPPERS > 7
        if((stepper_port_output & 0x8000) != (value & 0x8000))
        {
            // direction change on this axis
            if(0 == (value & 0x8000))
            {
                STEPPER_7_DIR_PORT &= ~STEPPER_7_DIR_MASK;
            }
            else
            {
                STEPPER_7_DIR_PORT |=  STEPPER_7_DIR_MASK;
            }
        }
        // else no direction change on this stepper
#endif
    }
    // else no change in direction signals

    // step
#if NUMBER_OF_STEPPERS > 0
    if((stepper_port_output & 0x01) != (value & 0x01))
    {
        // make a step on this axis
        STEPPER_0_STEP_PORT |= STEPPER_0_STEP_MASK;
    }
    // else no step on this stepper
#endif

#if NUMBER_OF_STEPPERS > 1
    if((stepper_port_output & 0x02) != (value & 0x02))
    {
        // make a step on this axis
        STEPPER_1_STEP_PORT |= STEPPER_1_STEP_MASK;
    }
    // else no step on this stepper
#endif

#if NUMBER_OF_STEPPERS > 2
    if((stepper_port_output & 0x04) != (value & 0x04))
    {
        // make a step on this axis
        STEPPER_2_STEP_PORT |= STEPPER_2_STEP_MASK;
    }
    // else no step on this stepper
#endif

#if NUMBER_OF_STEPPERS > 3
    if((stepper_port_output & 0x8) != (value & 0x08))
    {
        // make a step on this axis
        STEPPER_3_STEP_PORT |= STEPPER_3_STEP_MASK;
    }
    // else no step on this stepper
#endif

#if NUMBER_OF_STEPPERS > 4
    if((stepper_port_output & 0x10) != (value & 0x10))
    {
        // make a step on this axis
        STEPPER_4_STEP_PORT |= STEPPER_4_STEP_MASK;
    }
    // else no step on this stepper
#endif

#if NUMBER_OF_STEPPERS > 5
    if((stepper_port_output & 0x20) != (value & 0x20))
    {
        // make a step on this axis
        STEPPER_5_STEP_PORT |= STEPPER_5_STEP_MASK;
    }
    // else no step on this stepper
#endif

#if NUMBER_OF_STEPPERS > 6
    if((stepper_port_output & 0x40) != (value & 0x40))
    {
        // make a step on this axis
        STEPPER_6_STEP_PORT |= STEPPER_6_STEP_MASK;
    }
    // else no step on this stepper
#endif

#if NUMBER_OF_STEPPERS > 7
    if((stepper_port_output & 0x80) != (value & 0x80))
    {
        // make a step on this axis
        STEPPER_7_STEP_PORT |= STEPPER_7_STEP_MASK;
    }
    // else no step on this stepper
#endif

    // I probably don't need to wait here as the computation already took enough time

#if NUMBER_OF_STEPPERS > 0
    if((stepper_port_output & 0x01) != (value & 0x01))
    {
        // make a step on this axis
        STEPPER_0_STEP_PORT &= ~STEPPER_0_STEP_MASK;
    }
    // else no step on this stepper
#endif

#if NUMBER_OF_STEPPERS > 1
    if((stepper_port_output & 0x02) != (value & 0x02))
    {
        // make a step on this axis
        STEPPER_1_STEP_PORT &= ~STEPPER_1_STEP_MASK;
    }
    // else no step on this stepper
#endif

#if NUMBER_OF_STEPPERS > 2
    if((stepper_port_output & 0x04) != (value & 0x04))
    {
        // make a step on this axis
        STEPPER_2_STEP_PORT &= ~STEPPER_2_STEP_MASK;
    }
    // else no step on this stepper
#endif

#if NUMBER_OF_STEPPERS > 3
    if((stepper_port_output & 0x8) != (value & 0x08))
    {
        // make a step on this axis
        STEPPER_3_STEP_PORT &= ~STEPPER_3_STEP_MASK;
    }
    // else no step on this stepper
#endif

#if NUMBER_OF_STEPPERS > 4
    if((stepper_port_output & 0x10) != (value & 0x10))
    {
        // make a step on this axis
        STEPPER_4_STEP_PORT &= ~STEPPER_4_STEP_MASK;
    }
    // else no step on this stepper
#endif

#if NUMBER_OF_STEPPERS > 5
    if((stepper_port_output & 0x20) != (value & 0x20))
    {
        // make a step on this axis
        STEPPER_5_STEP_PORT &= ~STEPPER_5_STEP_MASK;
    }
    // else no step on this stepper
#endif

#if NUMBER_OF_STEPPERS > 6
    if((stepper_port_output & 0x40) != (value & 0x40))
    {
        // make a step on this axis
        STEPPER_6_STEP_PORT &= ~STEPPER_6_STEP_MASK;
    }
    // else no step on this stepper
#endif

#if NUMBER_OF_STEPPERS > 7
    if((stepper_port_output & 0x80) != (value & 0x80))
    {
        // make a step on this axis
        STEPPER_7_STEP_PORT &= ~STEPPER_7_STEP_MASK;
    }
    // else no step on this stepper
#endif
    stepper_port_output = value;
}

uint32_t hal_stepper_get_Output(void)
{
    return stepper_port_output;
}

