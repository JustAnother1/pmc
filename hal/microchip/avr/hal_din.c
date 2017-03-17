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
#include "hal_din.h"
#include "hal_debug.h"
#include "board_cfg.h"

void hal_din_init(void)
{
    // Enable Pull Up functionality
    MCUCR &= ~0x10; // PUD = 0
    // configure pins as Digital Input - pull up enabled
#if D_IN_NUM_PINS > 0
    D_IN_0_DDR &= ~D_IN_0_MASK;
    D_IN_0_PORT |= D_IN_0_MASK;
#endif
#if D_IN_NUM_PINS > 1
    D_IN_1_DDR &= ~D_IN_1_MASK;
    D_IN_1_PORT |= D_IN_1_MASK;
#endif
#if D_IN_NUM_PINS > 2
    D_IN_2_DDR &= ~D_IN_2_MASK;
    D_IN_2_PORT |= D_IN_2_MASK;
#endif
#if D_IN_NUM_PINS > 3
    D_IN_3_DDR &= ~D_IN_3_MASK;
    D_IN_3_PORT |= D_IN_3_MASK;
#endif
#if D_IN_NUM_PINS > 4
    D_IN_4_DDR &= ~D_IN_4_MASK;
    D_IN_4_PORT |= D_IN_4_MASK;
#endif
#if D_IN_NUM_PINS > 5
    D_IN_5_DDR &= ~D_IN_5_MASK;
    D_IN_5_PORT |= D_IN_5_MASK;
#endif
}

uint_fast8_t hal_din_get_amount(void)
{
    return D_IN_NUM_PINS;
}

uint_fast8_t hal_din_get_name(uint_fast8_t device, uint8_t *position, uint_fast8_t max_length)
{
    switch(device)
    {
#if D_IN_NUM_PINS > 0
    case  0: return strlcpy_P((char *)position, STR(D_IN_0_NAME), max_length);
#endif
#if D_IN_NUM_PINS > 1
    case  1: return strlcpy_P((char *)position, STR(D_IN_1_NAME), max_length);
#endif
#if D_IN_NUM_PINS > 2
    case  2: return strlcpy_P((char *)position, STR(D_IN_2_NAME), max_length);
#endif
#if D_IN_NUM_PINS > 3
    case  3: return strlcpy_P((char *)position, STR(D_IN_3_NAME), max_length);
#endif
#if D_IN_NUM_PINS > 4
    case  4: return strlcpy_P((char *)position, STR(D_IN_4_NAME), max_length);
#endif
#if D_IN_NUM_PINS > 5
    case  5: return strlcpy_P((char *)position, STR(D_IN_5_NAME), max_length);
#endif
#if D_IN_NUM_PINS > 6
    case  6: return strlcpy_P((char *)position, STR(D_IN_6_NAME), max_length);
#endif
#if D_IN_NUM_PINS > 7
    case  7: return strlcpy_P((char *)position, STR(D_IN_7_NAME), max_length);
#endif
    default:
        return 0;
    }
}

uint_fast8_t hal_din_get_switch_state(uint_fast8_t device)
{
    if(device < D_IN_NUM_PINS)
    {
        switch(device)
        {
#if D_IN_NUM_PINS > 0
        case 0: if(0 !=(D_IN_0_PIN & D_IN_0_MASK)) {return 1;} else {return 0;} break;
#endif
#if D_IN_NUM_PINS > 1
        case 1: if(0 !=(D_IN_1_PIN & D_IN_1_MASK)) {return 1;} else {return 0;} break;
#endif
#if D_IN_NUM_PINS > 2
        case 2: if(0 !=(D_IN_2_PIN & D_IN_2_MASK)) {return 1;} else {return 0;} break;
#endif
#if D_IN_NUM_PINS > 3
        case 3: if(0 !=(D_IN_3_PIN & D_IN_3_MASK)) {return 1;} else {return 0;} break;
#endif
#if D_IN_NUM_PINS > 4
        case 4: if(0 !=(D_IN_4_PIN & D_IN_4_MASK)) {return 1;} else {return 0;} break;
#endif
#if D_IN_NUM_PINS > 5
        case 5: if(0 !=(D_IN_5_PIN & D_IN_5_MASK)) {return 1;} else {return 0;} break;
#endif
        default:
            debug_line(STR("din pin(%d) not available!"), device);
            break;
        }
    }
    else
    {
        debug_line(STR("din pin(%d) not available!"), device);
    }
    return 0;
}

void hal_din_print_PinConfiguration(uint_fast8_t port, int idx)
{
    // TODO
}

void hal_din_subscribe_to_events(uint_fast8_t switch_number,
                                 uint_fast8_t stepper_number,
                                 din_func handle_func)
{
    // TODO
}

