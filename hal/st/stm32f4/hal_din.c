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
#include "hal_din.h"
#include "board_cfg.h"
#include "hal_debug.h"

void hal_din_init(void)
{
#if D_IN_NUM_PINS > 0
        RCC->AHB1ENR |= D_IN_0_RCC_GPIO_ENABLE;
        D_IN_0_GPIO_PORT->MODER   &= ~D_IN_0_MODER_0;
        D_IN_0_GPIO_PORT->MODER   |=  D_IN_0_MODER_1;
        D_IN_0_GPIO_PORT->OTYPER  &= ~D_IN_0_OTYPER_0;
        D_IN_0_GPIO_PORT->OTYPER  |=  D_IN_0_OTYPER_1;
        D_IN_0_GPIO_PORT->OSPEEDR &= ~D_IN_0_OSPEEDR_0;
        D_IN_0_GPIO_PORT->OSPEEDR |=  D_IN_0_OSPEEDR_1;
        D_IN_0_GPIO_PORT->PUPDR   &= ~D_IN_0_PUPD_0;
        D_IN_0_GPIO_PORT->PUPDR   |=  D_IN_0_PUPD_1;
#endif

#if D_IN_NUM_PINS > 1
        RCC->AHB1ENR |= D_IN_1_RCC_GPIO_ENABLE;
        D_IN_1_GPIO_PORT->MODER   &= ~D_IN_1_MODER_0;
        D_IN_1_GPIO_PORT->MODER   |=  D_IN_1_MODER_1;
        D_IN_1_GPIO_PORT->OTYPER  &= ~D_IN_1_OTYPER_0;
        D_IN_1_GPIO_PORT->OTYPER  |=  D_IN_1_OTYPER_1;
        D_IN_1_GPIO_PORT->OSPEEDR &= ~D_IN_1_OSPEEDR_0;
        D_IN_1_GPIO_PORT->OSPEEDR |=  D_IN_1_OSPEEDR_1;
        D_IN_1_GPIO_PORT->PUPDR   &= ~D_IN_1_PUPD_0;
        D_IN_1_GPIO_PORT->PUPDR   |=  D_IN_1_PUPD_1;
#endif

#if D_IN_NUM_PINS > 2
        RCC->AHB1ENR |= D_IN_2_RCC_GPIO_ENABLE;
        D_IN_2_GPIO_PORT->MODER   &= ~D_IN_2_MODER_0;
        D_IN_2_GPIO_PORT->MODER   |=  D_IN_2_MODER_1;
        D_IN_2_GPIO_PORT->OTYPER  &= ~D_IN_2_OTYPER_0;
        D_IN_2_GPIO_PORT->OTYPER  |=  D_IN_2_OTYPER_1;
        D_IN_2_GPIO_PORT->OSPEEDR &= ~D_IN_2_OSPEEDR_0;
        D_IN_2_GPIO_PORT->OSPEEDR |=  D_IN_2_OSPEEDR_1;
        D_IN_2_GPIO_PORT->PUPDR   &= ~D_IN_2_PUPD_0;
        D_IN_2_GPIO_PORT->PUPDR   |=  D_IN_2_PUPD_1;
#endif

#if D_IN_NUM_PINS > 3
        RCC->AHB1ENR |= D_IN_3_RCC_GPIO_ENABLE;
        D_IN_3_GPIO_PORT->MODER   &= ~D_IN_3_MODER_0;
        D_IN_3_GPIO_PORT->MODER   |=  D_IN_3_MODER_1;
        D_IN_3_GPIO_PORT->OTYPER  &= ~D_IN_3_OTYPER_0;
        D_IN_3_GPIO_PORT->OTYPER  |=  D_IN_3_OTYPER_1;
        D_IN_3_GPIO_PORT->OSPEEDR &= ~D_IN_3_OSPEEDR_0;
        D_IN_3_GPIO_PORT->OSPEEDR |=  D_IN_3_OSPEEDR_1;
        D_IN_3_GPIO_PORT->PUPDR   &= ~D_IN_3_PUPD_0;
        D_IN_3_GPIO_PORT->PUPDR   |=  D_IN_3_PUPD_1;
#endif

#if D_IN_NUM_PINS > 4
        RCC->AHB1ENR |= D_IN_4_RCC_GPIO_ENABLE;
        D_IN_4_GPIO_PORT->MODER   &= ~D_IN_4_MODER_0;
        D_IN_4_GPIO_PORT->MODER   |=  D_IN_4_MODER_1;
        D_IN_4_GPIO_PORT->OTYPER  &= ~D_IN_4_OTYPER_0;
        D_IN_4_GPIO_PORT->OTYPER  |=  D_IN_4_OTYPER_1;
        D_IN_4_GPIO_PORT->OSPEEDR &= ~D_IN_4_OSPEEDR_0;
        D_IN_4_GPIO_PORT->OSPEEDR |=  D_IN_4_OSPEEDR_1;
        D_IN_4_GPIO_PORT->PUPDR   &= ~D_IN_4_PUPD_0;
        D_IN_4_GPIO_PORT->PUPDR   |=  D_IN_4_PUPD_1;
#endif

#if D_IN_NUM_PINS > 5
        RCC->AHB1ENR |= D_IN_5_RCC_GPIO_ENABLE;
        D_IN_5_GPIO_PORT->MODER   &= ~D_IN_5_MODER_0;
        D_IN_5_GPIO_PORT->MODER   |=  D_IN_5_MODER_1;
        D_IN_5_GPIO_PORT->OTYPER  &= ~D_IN_5_OTYPER_0;
        D_IN_5_GPIO_PORT->OTYPER  |=  D_IN_5_OTYPER_1;
        D_IN_5_GPIO_PORT->OSPEEDR &= ~D_IN_5_OSPEEDR_0;
        D_IN_5_GPIO_PORT->OSPEEDR |=  D_IN_5_OSPEEDR_1;
        D_IN_5_GPIO_PORT->PUPDR   &= ~D_IN_5_PUPD_0;
        D_IN_5_GPIO_PORT->PUPDR   |=  D_IN_5_PUPD_1;
#endif
}

uint_fast8_t hal_din_get_amount(void)
{
    return D_IN_NUM_PINS;
}

uint_fast8_t hal_din_get_name(uint_fast8_t device, uint8_t *position)
{
    if(device < D_OUT_NUM_PINS)
    {
        switch(device)
        {
#if D_IN_NUM_PINS > 0
        case 0: return copy_string(D_IN_0_NAME, position);
#endif
#if D_IN_NUM_PINS > 1
        case 1: return copy_string(D_IN_1_NAME, position);
#endif
#if D_IN_NUM_PINS > 2
        case 2: return copy_string(D_IN_2_NAME, position);
#endif
#if D_IN_NUM_PINS > 3
        case 3: return copy_string(D_IN_3_NAME, position);
#endif
#if D_IN_NUM_PINS > 4
        case 4: return copy_string(D_IN_4_NAME, position);
#endif
#if D_IN_NUM_PINS > 5
        case 5: return copy_string(D_IN_5_NAME, position);
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

uint_fast8_t hal_din_get_switch_state(uint_fast8_t device)
{
    if(device < D_OUT_NUM_PINS)
    {
        switch(device)
        {
#if D_IN_NUM_PINS > 0
        case 0: if(0 !=(D_IN_0_GPIO_PORT->IDR & D_IN_0_IDR)) {return 1;} else {return 0;} break;
#endif
#if D_IN_NUM_PINS > 1
        case 1: if(0 !=(D_IN_1_GPIO_PORT->IDR & D_IN_1_IDR)) {return 1;} else {return 0;} break;
#endif
#if D_IN_NUM_PINS > 2
        case 2: if(0 !=(D_IN_2_GPIO_PORT->IDR & D_IN_2_IDR)) {return 1;} else {return 0;} break;
#endif
#if D_IN_NUM_PINS > 3
        case 3: if(0 !=(D_IN_3_GPIO_PORT->IDR & D_IN_3_IDR)) {return 1;} else {return 0;} break;
#endif
#if D_IN_NUM_PINS > 4
        case 4: if(0 !=(D_IN_4_GPIO_PORT->IDR & D_IN_4_IDR)) {return 1;} else {return 0;} break;
#endif
#if D_IN_NUM_PINS > 5
        case 5: if(0 !=(D_IN_5_GPIO_PORT->IDR & D_IN_5_IDR)) {return 1;} else {return 0;} break;
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
    return 0;
}

void hal_din_print_PinConfiguration(uint_fast8_t port, int idx)
{
    GPIO_TypeDef * PortRegisters;
    debug_line("Configuration of pin %c %d :", port, idx);
    switch(port)
    {
    case 'A':
    case 'a':
        PortRegisters = GPIOA;
        break;

    case 'B':
    case 'b':
        PortRegisters = GPIOB;
        break;

    case 'C':
    case 'c':
        PortRegisters = GPIOC;
        break;

    case 'D':
    case 'd':
        PortRegisters = GPIOD;
        break;

    case 'E':
    case 'e':
        PortRegisters = GPIOE;
        break;

    case 'F':
    case 'f':
        PortRegisters = GPIOF;
        break;

    case 'G':
    case 'g':
        PortRegisters = GPIOG;
        break;

    case 'H':
    case 'h':
        PortRegisters = GPIOH;
        break;

    case 'I':
    case 'i':
        PortRegisters = GPIOI;
        break;

    default:
        debug_line("Invalid PortRegisters !");
        return;
    }
    print_gpio_pin_configuration(PortRegisters, idx);
}


