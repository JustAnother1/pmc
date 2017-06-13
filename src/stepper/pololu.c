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
#include "step.h"
#include "board_cfg.h"

// TODO move to hal - and then use hal_dout

void pololu_init(void)
{
#if NUMBER_OF_STEPPERS > 0
    // Enable is inverted
    STEPPER_0_ENABLE_DDR  |=  STEPPER_0_ENABLE_MASK;
    STEPPER_0_ENABLE_PORT |=  STEPPER_0_ENABLE_MASK;
    STEPPER_0_STEP_DDR    |=  STEPPER_0_STEP_MASK;
    STEPPER_0_STEP_PORT   &= ~STEPPER_0_STEP_MASK;
    STEPPER_0_DIR_DDR     |=  STEPPER_0_DIR_MASK;
    STEPPER_0_DIR_PORT    &= ~STEPPER_0_DIR_MASK;
#endif
#if NUMBER_OF_STEPPERS > 1
    // Enable is inverted
    STEPPER_1_ENABLE_DDR  |=  STEPPER_1_ENABLE_MASK;
    STEPPER_1_ENABLE_PORT |=  STEPPER_1_ENABLE_MASK;
    STEPPER_1_STEP_DDR    |=  STEPPER_1_STEP_MASK;
    STEPPER_1_STEP_PORT   &= ~STEPPER_1_STEP_MASK;
    STEPPER_1_DIR_DDR     |=  STEPPER_1_DIR_MASK;
    STEPPER_1_DIR_PORT    &= ~STEPPER_1_DIR_MASK;
#endif
#if NUMBER_OF_STEPPERS > 2
    // Enable is inverted
    STEPPER_2_ENABLE_DDR  |=  STEPPER_2_ENABLE_MASK;
    STEPPER_2_ENABLE_PORT |=  STEPPER_2_ENABLE_MASK;
    STEPPER_2_STEP_DDR    |=  STEPPER_2_STEP_MASK;
    STEPPER_2_STEP_PORT   &= ~STEPPER_2_STEP_MASK;
    STEPPER_2_DIR_DDR     |=  STEPPER_2_DIR_MASK;
    STEPPER_2_DIR_PORT    &= ~STEPPER_2_DIR_MASK;
#endif
#if NUMBER_OF_STEPPERS > 3
    // Enable is inverted
    STEPPER_3_ENABLE_DDR  |=  STEPPER_3_ENABLE_MASK;
    STEPPER_3_ENABLE_PORT |=  STEPPER_3_ENABLE_MASK;
    STEPPER_3_STEP_DDR    |=  STEPPER_3_STEP_MASK;
    STEPPER_3_STEP_PORT   &= ~STEPPER_3_STEP_MASK;
    STEPPER_3_DIR_DDR     |=  STEPPER_3_DIR_MASK;
    STEPPER_3_DIR_PORT    &= ~STEPPER_3_DIR_MASK;
#endif
#if NUMBER_OF_STEPPERS > 4
    // Enable is inverted
    STEPPER_4_ENABLE_DDR  |=  STEPPER_4_ENABLE_MASK;
    STEPPER_4_ENABLE_PORT |=  STEPPER_4_ENABLE_MASK;
    STEPPER_4_STEP_DDR    |=  STEPPER_4_STEP_MASK;
    STEPPER_4_STEP_PORT   &= ~STEPPER_4_STEP_MASK;
    STEPPER_4_DIR_DDR     |=  STEPPER_4_DIR_MASK;
    STEPPER_4_DIR_PORT    &= ~STEPPER_4_DIR_MASK;
#endif
#if NUMBER_OF_STEPPERS > 5
    // Enable is inverted
    STEPPER_5_ENABLE_DDR  |=  STEPPER_5_ENABLE_MASK;
    STEPPER_5_ENABLE_PORT |=  STEPPER_5_ENABLE_MASK;
    STEPPER_5_STEP_DDR    |=  STEPPER_5_STEP_MASK;
    STEPPER_5_STEP_PORT   &= ~STEPPER_5_STEP_MASK;
    STEPPER_5_DIR_DDR     |=  STEPPER_5_DIR_MASK;
    STEPPER_5_DIR_PORT    &= ~STEPPER_5_DIR_MASK;
#endif
#if NUMBER_OF_STEPPERS > 6
    // Enable is inverted
    STEPPER_6_ENABLE_DDR  |=  STEPPER_6_ENABLE_MASK;
    STEPPER_6_ENABLE_PORT |=  STEPPER_6_ENABLE_MASK;
    STEPPER_6_STEP_DDR    |=  STEPPER_6_STEP_MASK;
    STEPPER_6_STEP_PORT   &= ~STEPPER_6_STEP_MASK;
    STEPPER_6_DIR_DDR     |=  STEPPER_6_DIR_MASK;
    STEPPER_6_DIR_PORT    &= ~STEPPER_6_DIR_MASK;
#endif
#if NUMBER_OF_STEPPERS > 7
    // Enable is inverted
    STEPPER_7_ENABLE_DDR  |=  STEPPER_7_ENABLE_MASK;
    STEPPER_7_ENABLE_PORT |=  STEPPER_7_ENABLE_MASK;
    STEPPER_7_STEP_DDR    |=  STEPPER_7_STEP_MASK;
    STEPPER_7_STEP_PORT   &= ~STEPPER_7_STEP_MASK;
    STEPPER_7_DIR_DDR     |=  STEPPER_7_DIR_MASK;
    STEPPER_7_DIR_PORT    &= ~STEPPER_7_DIR_MASK;
#endif
}

void pololu_enable_motor(uint_fast8_t stepper_number)
{
    if(stepper_number < NUMBER_OF_STEPPERS)
    {
        switch(stepper_number)
        {
    #if NUMBER_OF_STEPPERS > 0
        case  0:
            // debug_line(STR("enabling Motor 0!"));
            STEPPER_0_ENABLE_PORT &= ~STEPPER_0_ENABLE_MASK;
            break;
    #endif
    #if NUMBER_OF_STEPPERS > 1
        case  1:
            // debug_line(STR("enabling Motor 1!"));
            STEPPER_1_ENABLE_PORT &= ~STEPPER_1_ENABLE_MASK;
            break;
    #endif
    #if NUMBER_OF_STEPPERS > 2
        case  2:
            // debug_line(STR("enabling Motor 2!"));
            STEPPER_2_ENABLE_PORT &= ~STEPPER_2_ENABLE_MASK;
            break;
    #endif
    #if NUMBER_OF_STEPPERS > 3
        case  3:
            // debug_line(STR("enabling Motor 3!"));
            STEPPER_3_ENABLE_PORT &= ~STEPPER_3_ENABLE_MASK;
            break;
    #endif
    #if NUMBER_OF_STEPPERS > 4
        case  4:
            // debug_line(STR("enabling Motor 4!"));
            STEPPER_4_ENABLE_PORT &= ~STEPPER_4_ENABLE_MASK;
            break;
    #endif
    #if NUMBER_OF_STEPPERS > 5
        case  5:
            // debug_line(STR("enabling Motor 5!"));
            STEPPER_5_ENABLE_PORT &= ~STEPPER_5_ENABLE_MASK;
            break;
    #endif
    #if NUMBER_OF_STEPPERS > 6
        case  6:
            // debug_line(STR("enabling Motor 6!"));
            STEPPER_6_ENABLE_PORT &= ~STEPPER_6_ENABLE_MASK;
            break;
    #endif
    #if NUMBER_OF_STEPPERS > 7
        case  7:
            // debug_line(STR("enabling Motor 7!"));
            STEPPER_7_ENABLE_PORT &= ~STEPPER_7_ENABLE_MASK;
            break;
    #endif
        }
    }
    // else this board does not have that stepper
}

void pololu_disable_motor(uint_fast8_t stepper_number)
{
    if(stepper_number < NUMBER_OF_STEPPERS)
    {
        switch(stepper_number)
        {
    #if NUMBER_OF_STEPPERS > 0
        case  0:
            // debug_line(STR("disabling Motor 0!"));
            STEPPER_0_ENABLE_PORT |= STEPPER_0_ENABLE_MASK;
            break;
    #endif
    #if NUMBER_OF_STEPPERS > 1
        case  1:
            // debug_line(STR("disabling Motor 1!"));
            STEPPER_1_ENABLE_PORT |= STEPPER_1_ENABLE_MASK;
            break;
    #endif
    #if NUMBER_OF_STEPPERS > 2
        case  2:
            // debug_line(STR("disabling Motor 2!"));
            STEPPER_2_ENABLE_PORT |= STEPPER_2_ENABLE_MASK;
            break;
    #endif
    #if NUMBER_OF_STEPPERS > 3
        case  3:
            // debug_line(STR("disabling Motor 3!"));
            STEPPER_3_ENABLE_PORT |= STEPPER_3_ENABLE_MASK;
            break;
    #endif
    #if NUMBER_OF_STEPPERS > 4
        case  4:
            // debug_line(STR("disabling Motor 4!"));
            STEPPER_4_ENABLE_PORT |= STEPPER_4_ENABLE_MASK;
            break;
    #endif
    #if NUMBER_OF_STEPPERS > 5
        case  5:
            // debug_line(STR("disabling Motor 5!"));
            STEPPER_5_ENABLE_PORT |= STEPPER_5_ENABLE_MASK;
            break;
    #endif
    #if NUMBER_OF_STEPPERS > 6
        case  6:
            // debug_line(STR("disabling Motor 6!"));
            STEPPER_6_ENABLE_PORT |= STEPPER_6_ENABLE_MASK;
            break;
    #endif
    #if NUMBER_OF_STEPPERS > 7
        case  7:
            // debug_line(STR("disabling Motor 7!"));
            STEPPER_7_ENABLE_PORT |= STEPPER_7_ENABLE_MASK;
            break;
    #endif
        }
    }
    // else this board does not have that stepper
}

#ifdef DEBUG_ACTIVE
void pololu_print_status()
{
    step_print_state();
}
#endif

