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

#include "hal_dout.h"
#include "board_cfg.h"
#include "rcc.h"
#include "gpio.h"
#include "debug.h"

void hal_dout_init(void)
{
    if(D_OUT_NUM_PINS > 0)
    {
        RCC->AHB1ENR |= D_OUT_0_RCC_GPIO_ENABLE;
        // LEDS are general purpose outputs
        D_OUT_0_GPIO_PORT->MODER |= D_OUT_0_MODER;
        // LEDS are push pull
        D_OUT_0_GPIO_PORT->OTYPER &= ~D_OUT_0_OTYPER;
        // LEDS are low speed (= max 2MHz) but high current
        D_OUT_0_GPIO_PORT->OSPEEDR &= ~D_OUT_0_OSPEEDR;
        // No pull ups or pull downs
        D_OUT_0_GPIO_PORT->PUPDR &= ~ D_OUT_0_PUPD;
        // start with output = 0
        D_OUT_0_GPIO_PORT->ODR &= D_OUT_0_ODR;
    }
    if(D_OUT_NUM_PINS > 1)
    {
        RCC->AHB1ENR |= D_OUT_1_RCC_GPIO_ENABLE;
        // LEDS are general purpose outputs
        D_OUT_1_GPIO_PORT->MODER |= D_OUT_1_MODER;
        // LEDS are push pull
        D_OUT_1_GPIO_PORT->OTYPER &= ~D_OUT_1_OTYPER;
        // LEDS are low speed (= max 2MHz) but high current
        D_OUT_1_GPIO_PORT->OSPEEDR &= ~D_OUT_1_OSPEEDR;
        // No pull ups or pull downs
        D_OUT_1_GPIO_PORT->PUPDR &= ~ D_OUT_1_PUPD;
        // start with output = 0
        D_OUT_1_GPIO_PORT->ODR &= D_OUT_1_ODR;
    }
}

uint_fast8_t hal_dout_get_amount(void)
{
    return D_OUT_NUM_PINS;
}

void hal_dout_set_pin_high(uint_fast8_t number)
{
    if(number < D_OUT_NUM_PINS)
    {
        switch(number)
        {
        case 0: D_OUT_0_GPIO_PORT->ODR |= D_OUT_0_ODR; break;
        case 1: D_OUT_1_GPIO_PORT->ODR |= D_OUT_1_ODR; break;
        default:
            debug_msg("dout pin(%d) not available!\n", number);
            break;
        }
    }
    else
    {
        debug_msg("dout pin(%d) not available!\n", number);
    }
}

void hal_dout_set_pin_low(uint_fast8_t number)
{
    if(number < D_OUT_NUM_PINS)
    {
        switch(number)
        {
        case 0: D_OUT_0_GPIO_PORT->ODR &= ~D_OUT_0_ODR; break;
        case 1: D_OUT_1_GPIO_PORT->ODR &= ~D_OUT_1_ODR; break;
        default:
            debug_msg("dout pin(%d) not available!\n", number);
            break;
        }
    }
    else
    {
        debug_msg("dout pin(%d) not available!\n", number);
    }
}

static uint_fast8_t copy_string(char * str, uint8_t *position)
{
    uint_fast8_t num = 0;
    char c = str[num];
    while(c !=0)
    {
        *position = c;
        num++;
        c = str[num];
    }
    return num;
}

uint_fast8_t hal_dout_get_name(uint_fast8_t number, uint8_t *position)
{
    if(number < D_OUT_NUM_PINS)
    {
        switch(number)
        {
        case 0: return copy_string(D_OUT_0_NAME, position);
        case 1: return copy_string(D_OUT_1_NAME, position);
        default:
            return 0;
        }
    }
    else
    {
        return 0;
    }
}

