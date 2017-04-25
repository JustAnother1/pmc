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

#include <stdbool.h>
#include <stdio.h>
#include "hal_din.h"
#include "hal_debug.h"
#include "board_cfg.h"

static bool initialized = false;
static din_func funcs[D_IN_NUM_PINS];
static uint_fast8_t steppers[D_IN_NUM_PINS];
static uint_fast8_t last_state[D_IN_NUM_PINS];

#define STABLE_CNT  5
static uint_fast8_t debounce_cnt[D_IN_NUM_PINS];
static uint_fast8_t pin_state[D_IN_NUM_PINS];

void hal_din_init(void)
{
    int i;
    if(true == initialized)
    {
        // initialize only once !
        return;
    }
    initialized = true;

    for(i = 0; i < D_IN_NUM_PINS; i++)
    {
        funcs[i] = NULL;
        steppers[i] = 0xff;
        last_state[i] = 0;
        pin_state[i] = 0;
        debounce_cnt[i] = 0;
    }

    // Enable Pull Up functionality
    MCUCR &= ~0x10; // PUD = 0
    // configure pins as Digital Input - pull up enabled
#if D_IN_NUM_PINS > 0
    D_IN_0_PORT |= D_IN_0_MASK;
#endif
#if D_IN_NUM_PINS > 1
    D_IN_1_PORT |= D_IN_1_MASK;
#endif
#if D_IN_NUM_PINS > 2
    D_IN_2_PORT |= D_IN_2_MASK;
#endif
#if D_IN_NUM_PINS > 3
    D_IN_3_PORT |= D_IN_3_MASK;
#endif
#if D_IN_NUM_PINS > 4
    D_IN_4_PORT |= D_IN_4_MASK;
#endif
#if D_IN_NUM_PINS > 5
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
        uint8_t cur_state;
        switch(device)
        {
#if D_IN_NUM_PINS > 0
        case 0:
            cur_state = (D_IN_0_PIN & D_IN_0_MASK);
            break;
#endif
#if D_IN_NUM_PINS > 1
        case 1:
            cur_state = (D_IN_1_PIN & D_IN_1_MASK);
            break;
#endif
#if D_IN_NUM_PINS > 2
        case 2:
            cur_state = (D_IN_2_PIN & D_IN_2_MASK);
            break;
#endif
#if D_IN_NUM_PINS > 3
        case 3:
            cur_state = (D_IN_3_PIN & D_IN_3_MASK);
            break;
#endif
#if D_IN_NUM_PINS > 4
        case 4:
            cur_state = (D_IN_4_PIN & D_IN_4_MASK);
            break;
#endif
#if D_IN_NUM_PINS > 5
        case 5:
            cur_state = (D_IN_5_PIN & D_IN_5_MASK);
            break;
#endif
        default:
            debug_line(STR("din pin(%d) not available!"), device);
            return DIN_ERROR;
            break;
        }

        if(0 != cur_state)
        {
            // Pin is high
            if(pin_state[device] == 1)
            {
                // Pin was high before
                debounce_cnt[device] = 0;
                return DIN_HIGH;
            }
            else
            {
                // Pin was low
                debounce_cnt[device] ++;
                if(STABLE_CNT < debounce_cnt[device])
                {
                    // Pin change is now stable 0 -> 1
                    pin_state[device] = 1;
                    debounce_cnt[device] = 0;
                    return DIN_HIGH;
                }
                else
                {
                    // not stable enough - report previous level
                    return DIN_LOW;
                }
            }
        }
        else
        {
            // Pin is low
            if(pin_state[device] == 0)
            {
                // Pin was low
                debounce_cnt[device] = 0;
                return DIN_LOW;
            }
            else
            {
                // Pin was high
                debounce_cnt[device] ++;
                if(STABLE_CNT < debounce_cnt[device])
                {
                    // pin change is now stable 1-> 0
                    pin_state[device] = 0;
                    debounce_cnt[device] = 0;
                    return DIN_LOW;
                }
                else
                {
                    // not stable enough - report previous level
                    return DIN_HIGH;
                }
            }
        }
    }
    else
    {
        debug_line(STR("din pin(%d) not available!"), device);
    }
    return DIN_ERROR;
}

void hal_din_print_PinConfiguration(uint_fast8_t port, int idx)
{
    (void) port;
    (void) idx;
    // TODO
}

void hal_din_subscribe_to_events(uint_fast8_t switch_number,
                                 uint_fast8_t stepper_number,
                                 din_func handle_func)
{
    funcs[switch_number] = handle_func;
    steppers[switch_number] = stepper_number;
}

#ifdef POLL_END_STOPS
void hal_din_poll(void)
{
    int i;
    int cur_state;
    for(i = 0; i < D_IN_NUM_PINS; i++)
    {
        if(NULL != funcs[i])
        {
            cur_state = hal_din_get_switch_state(i);
            if(last_state[i] != cur_state)
            {
                // this pin had a change
                last_state[i] = cur_state;
                debug_line(STR("Notify step"));
                if(1 == cur_state)
                {
                    funcs[i](true,steppers[i], i);
                }
                else
                {
                    funcs[i](false,steppers[i], i);
                }
            }
            // else no change on this pin
        }
        // else this pin is not subscribed
    }
}

void hal_din_tick(void)
{
    int i;
    int cur_state;
    for(i = 0; i < D_IN_NUM_PINS; i++)
    {
        cur_state = hal_din_get_switch_state(i);
    }
}
#endif

