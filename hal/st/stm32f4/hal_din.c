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
#include "board_cfg.h"
#include "rcc.h"
#include "gpio.h"
#include "util.h"
#include "hal_debug.h"

void hal_din_init(void)
{
    if(D_IN_NUM_PINS > 0)
    {
        RCC->AHB1ENR |= D_IN_0_RCC_GPIO_ENABLE;
        D_IN_0_GPIO_PORT->MODER   &= ~D_IN_0_MODER_0;
        D_IN_0_GPIO_PORT->MODER   |=  D_IN_0_MODER_1;
        D_IN_0_GPIO_PORT->OTYPER  &= ~D_IN_0_OTYPER_0;
        D_IN_0_GPIO_PORT->OTYPER  |=  D_IN_0_OTYPER_1;
        D_IN_0_GPIO_PORT->OSPEEDR &= ~D_IN_0_OSPEEDR_0;
        D_IN_0_GPIO_PORT->OSPEEDR |=  D_IN_0_OSPEEDR_1;
        D_IN_0_GPIO_PORT->PUPDR   &= ~D_IN_0_PUPD_0;
        D_IN_0_GPIO_PORT->PUPDR   |=  D_IN_0_PUPD_1;
    }
    if(D_IN_NUM_PINS > 1)
    {
        RCC->AHB1ENR |= D_IN_1_RCC_GPIO_ENABLE;
        D_IN_1_GPIO_PORT->MODER   &= ~D_IN_1_MODER_0;
        D_IN_1_GPIO_PORT->MODER   |=  D_IN_1_MODER_1;
        D_IN_1_GPIO_PORT->OTYPER  &= ~D_IN_1_OTYPER_0;
        D_IN_1_GPIO_PORT->OTYPER  |=  D_IN_1_OTYPER_1;
        D_IN_1_GPIO_PORT->OSPEEDR &= ~D_IN_1_OSPEEDR_0;
        D_IN_1_GPIO_PORT->OSPEEDR |=  D_IN_1_OSPEEDR_1;
        D_IN_1_GPIO_PORT->PUPDR   &= ~D_IN_1_PUPD_0;
        D_IN_1_GPIO_PORT->PUPDR   |=  D_IN_1_PUPD_1;
    }
}

uint_fast8_t hal_din_get_amount(void)
{
    return D_IN_NUM_PINS;
}

uint_fast8_t hal_din_get_name(uint_fast8_t number, uint8_t *position)
{
    if(number < D_OUT_NUM_PINS)
    {
        switch(number)
        {
        case 0: return copy_string(D_IN_0_NAME, position);
        case 1: return copy_string(D_IN_1_NAME, position);
        default:
            return 0;
        }
    }
    else
    {
        return 0;
    }
}

uint_fast8_t hal_din_get_switch_state(uint_fast8_t number)
{
    if(number < D_OUT_NUM_PINS)
    {
        switch(number)
        {
        case 0: if(0 !=(D_IN_0_GPIO_PORT->IDR & D_IN_0_IDR)) {return 1;} else {return 0;} break;
        case 1: if(0 !=(D_IN_1_GPIO_PORT->IDR & D_IN_1_IDR)) {return 1;} else {return 0;} break;
        default:
            debug_line("dout pin(%d) not available!", number);
            break;
        }
    }
    else
    {
        debug_line("dout pin(%d) not available!", number);
    }
    return 0;
}



