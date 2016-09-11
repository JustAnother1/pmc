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

#include <st_gpio.h>
#include <st_rcc.h>
#include <st_util.h>
#include "hal_dout.h"
#include "board_cfg.h"
#include "hal_debug.h"
#include "protocol.h"

static uint_fast8_t curState[D_OUT_NUM_PINS];

void hal_dout_init(void)
{
#if(D_OUT_NUM_PINS > 0)
        RCC->AHB1ENR |= D_OUT_0_RCC_GPIO_ENABLE;
        D_OUT_0_GPIO_PORT->MODER   &= ~D_OUT_0_MODER_0;
        D_OUT_0_GPIO_PORT->MODER   |=  D_OUT_0_MODER_1;
        D_OUT_0_GPIO_PORT->OTYPER  &= ~D_OUT_0_OTYPER_0;
        D_OUT_0_GPIO_PORT->OTYPER  |=  D_OUT_0_OTYPER_1;
        D_OUT_0_GPIO_PORT->OSPEEDR &= ~D_OUT_0_OSPEEDR_0;
        D_OUT_0_GPIO_PORT->OSPEEDR |=  D_OUT_0_OSPEEDR_1;
        D_OUT_0_GPIO_PORT->PUPDR   &= ~D_OUT_0_PUPD_0;
        D_OUT_0_GPIO_PORT->PUPDR   |=  D_OUT_0_PUPD_1;
        D_OUT_0_GPIO_PORT->ODR     &= ~D_OUT_0_ODR;
#endif
#if(D_OUT_NUM_PINS > 1)
        RCC->AHB1ENR |= D_OUT_1_RCC_GPIO_ENABLE;
        D_OUT_1_GPIO_PORT->MODER   &= ~D_OUT_1_MODER_0;
        D_OUT_1_GPIO_PORT->MODER   |=  D_OUT_1_MODER_1;
        D_OUT_1_GPIO_PORT->OTYPER  &= ~D_OUT_1_OTYPER_0;
        D_OUT_1_GPIO_PORT->OTYPER  |=  D_OUT_1_OTYPER_1;
        D_OUT_1_GPIO_PORT->OSPEEDR &= ~D_OUT_1_OSPEEDR_0;
        D_OUT_1_GPIO_PORT->OSPEEDR |=  D_OUT_1_OSPEEDR_1;
        D_OUT_1_GPIO_PORT->PUPDR   &= ~D_OUT_1_PUPD_0;
        D_OUT_1_GPIO_PORT->PUPDR   |=  D_OUT_1_PUPD_1;
        D_OUT_1_GPIO_PORT->ODR     &= ~D_OUT_1_ODR;
#endif
#if(D_OUT_NUM_PINS > 2)
        RCC->AHB1ENR |= D_OUT_2_RCC_GPIO_ENABLE;
        D_OUT_2_GPIO_PORT->MODER   &= ~D_OUT_2_MODER_0;
        D_OUT_2_GPIO_PORT->MODER   |=  D_OUT_2_MODER_1;
        D_OUT_2_GPIO_PORT->OTYPER  &= ~D_OUT_2_OTYPER_0;
        D_OUT_2_GPIO_PORT->OTYPER  |=  D_OUT_2_OTYPER_1;
        D_OUT_2_GPIO_PORT->OSPEEDR &= ~D_OUT_2_OSPEEDR_0;
        D_OUT_2_GPIO_PORT->OSPEEDR |=  D_OUT_2_OSPEEDR_1;
        D_OUT_2_GPIO_PORT->PUPDR   &= ~D_OUT_2_PUPD_0;
        D_OUT_2_GPIO_PORT->PUPDR   |=  D_OUT_2_PUPD_1;
        D_OUT_2_GPIO_PORT->ODR     &= ~D_OUT_2_ODR;
#endif
}

uint_fast8_t hal_dout_get_amount(void)
{
    return D_OUT_NUM_PINS;
}

void hal_dout_set_pin_high(uint_fast8_t device)
{
    if(device < D_OUT_NUM_PINS)
    {
        switch(device)
        {
#if(D_OUT_NUM_PINS > 0)
        case 0: D_OUT_0_GPIO_PORT->ODR |= D_OUT_0_ODR; curState[device] = OUTPUT_STATE_HIGH; break;
#endif
#if(D_OUT_NUM_PINS > 1)
        case 1: D_OUT_1_GPIO_PORT->ODR |= D_OUT_1_ODR; curState[device] = OUTPUT_STATE_HIGH; break;
#endif
#if(D_OUT_NUM_PINS > 2)
        case 2: D_OUT_2_GPIO_PORT->ODR |= D_OUT_2_ODR; curState[device] = OUTPUT_STATE_HIGH; break;
#endif
        default:
            debug_line("dout pin(%d) not available!", device);
            break;
        }
    }
    else
    {
        debug_line("dout pin(%d) not available!", device);
    }
}

void hal_dout_set_pin_low(uint_fast8_t device)
{
    if(device < D_OUT_NUM_PINS)
    {
        switch(device)
        {
#if(D_OUT_NUM_PINS > 0)
        case 0: D_OUT_0_GPIO_PORT->ODR &= ~D_OUT_0_ODR; curState[device] = OUTPUT_STATE_LOW; break;
#endif
#if(D_OUT_NUM_PINS > 1)
        case 1: D_OUT_1_GPIO_PORT->ODR &= ~D_OUT_1_ODR; curState[device] = OUTPUT_STATE_LOW; break;
#endif
#if(D_OUT_NUM_PINS > 2)
        case 2: D_OUT_2_GPIO_PORT->ODR &= ~D_OUT_2_ODR; curState[device] = OUTPUT_STATE_LOW; break;
#endif
        default:
            debug_line("dout pin(%d) not available!", device);
            break;
        }
    }
    else
    {
        debug_line("dout pin(%d) not available!", device);
    }
}

void hal_dout_set_pin_HighZ(uint_fast8_t device)
{
    // OUTPUT_STATE_HIGH_Z not meaningful on pipy
    debug_line("high-Z:not implemented");
}

uint_fast8_t hal_dout_get_name(uint_fast8_t device, uint8_t *position)
{
    if(device < D_OUT_NUM_PINS)
    {
        switch(device)
        {
#if(D_OUT_NUM_PINS > 0)
        case 0: return copy_string(D_OUT_0_NAME, position);
#endif
#if(D_OUT_NUM_PINS > 1)
        case 1: return copy_string(D_OUT_1_NAME, position);
#endif
#if(D_OUT_NUM_PINS > 2)
        case 2: return copy_string(D_OUT_2_NAME, position);
#endif
        default:
            return 0;
        }
    }
    else
    {
        return 0;
    }
}

uint_fast8_t hal_dout_get_current_state_of(uint_fast8_t number)
{
    return curState[number];
}

