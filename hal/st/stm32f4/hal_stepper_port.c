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
#include <st_gpio.h>
#include <st_rcc.h>
#include "hal_debug.h"
#include "hal_stepper_port.h"
#include "board_cfg.h"

static bool initialized = false;

void hal_stepper_port_init(void)
{
    if(true == initialized)
    {
        // initialize only once !
        return;
    }
    initialized = true;

    RCC->AHB1ENR |= STEPPER_PORT_RCC_GPIO_ENABLE;
    // make all used bit 0
    STEPPER_PORT_GPIO_PORT->MODER   &= ~USED_PINS_MASK;
    // set the needed bits to 1
    STEPPER_PORT_GPIO_PORT->MODER   |= (USED_PINS_MASK & STEPPER_PORT_MODER);
    STEPPER_PORT_GPIO_PORT->OTYPER  &= ~USED_PINS_MASK;
    STEPPER_PORT_GPIO_PORT->OTYPER  |= (USED_PINS_MASK & STEPPER_PORT_OTYPER);
    STEPPER_PORT_GPIO_PORT->OSPEEDR &= ~USED_PINS_MASK;
    STEPPER_PORT_GPIO_PORT->OSPEEDR |= (USED_PINS_MASK & STEPPER_PORT_OSPEEDR);
    STEPPER_PORT_GPIO_PORT->PUPDR   &= ~USED_PINS_MASK;
    STEPPER_PORT_GPIO_PORT->PUPDR   |= (USED_PINS_MASK & STEPPER_PORT_PUPD);
    STEPPER_PORT_GPIO_PORT->ODR     &= ~USED_PINS_MASK;
    STEPPER_PORT_GPIO_PORT->ODR     |= (USED_PINS_MASK & STEPPER_PORT_ODR);

#ifndef HAS_TRINAMIC

#if(NUMBER_OF_STEPPERS > 0)
    RCC->AHB1ENR |= STEPPER_ENABLE_0_RCC_GPIO_ENABLE;
    STEPPER_ENABLE_0_GPIO_PORT->MODER   &= ~STEPPER_ENABLE_0_MODER_0;
    STEPPER_ENABLE_0_GPIO_PORT->MODER   |=  STEPPER_ENABLE_0_MODER_1;
    STEPPER_ENABLE_0_GPIO_PORT->OTYPER  &= ~STEPPER_ENABLE_0_OTYPER_0;
    STEPPER_ENABLE_0_GPIO_PORT->OTYPER  |=  STEPPER_ENABLE_0_OTYPER_1;
    STEPPER_ENABLE_0_GPIO_PORT->OSPEEDR &= ~STEPPER_ENABLE_0_OSPEEDR_0;
    STEPPER_ENABLE_0_GPIO_PORT->OSPEEDR |=  STEPPER_ENABLE_0_OSPEEDR_1;
    STEPPER_ENABLE_0_GPIO_PORT->PUPDR   &= ~STEPPER_ENABLE_0_PUPD_0;
    STEPPER_ENABLE_0_GPIO_PORT->PUPDR   |=  STEPPER_ENABLE_0_PUPD_1;
    STEPPER_ENABLE_0_GPIO_PORT->ODR     &= ~STEPPER_ENABLE_0_ODR;
#endif

#if(NUMBER_OF_STEPPERS > 1)
    RCC->AHB1ENR |= STEPPER_ENABLE_1_RCC_GPIO_ENABLE;
    STEPPER_ENABLE_1_GPIO_PORT->MODER   &= ~STEPPER_ENABLE_1_MODER_0;
    STEPPER_ENABLE_1_GPIO_PORT->MODER   |=  STEPPER_ENABLE_1_MODER_1;
    STEPPER_ENABLE_1_GPIO_PORT->OTYPER  &= ~STEPPER_ENABLE_1_OTYPER_0;
    STEPPER_ENABLE_1_GPIO_PORT->OTYPER  |=  STEPPER_ENABLE_1_OTYPER_1;
    STEPPER_ENABLE_1_GPIO_PORT->OSPEEDR &= ~STEPPER_ENABLE_1_OSPEEDR_0;
    STEPPER_ENABLE_1_GPIO_PORT->OSPEEDR |=  STEPPER_ENABLE_1_OSPEEDR_1;
    STEPPER_ENABLE_1_GPIO_PORT->PUPDR   &= ~STEPPER_ENABLE_1_PUPD_0;
    STEPPER_ENABLE_1_GPIO_PORT->PUPDR   |=  STEPPER_ENABLE_1_PUPD_1;
    STEPPER_ENABLE_1_GPIO_PORT->ODR     &= ~STEPPER_ENABLE_1_ODR;
#endif

#if(NUMBER_OF_STEPPERS > 2)
    RCC->AHB1ENR |= STEPPER_ENABLE_2_RCC_GPIO_ENABLE;
    STEPPER_ENABLE_2_GPIO_PORT->MODER   &= ~STEPPER_ENABLE_2_MODER_0;
    STEPPER_ENABLE_2_GPIO_PORT->MODER   |=  STEPPER_ENABLE_2_MODER_1;
    STEPPER_ENABLE_2_GPIO_PORT->OTYPER  &= ~STEPPER_ENABLE_2_OTYPER_0;
    STEPPER_ENABLE_2_GPIO_PORT->OTYPER  |=  STEPPER_ENABLE_2_OTYPER_1;
    STEPPER_ENABLE_2_GPIO_PORT->OSPEEDR &= ~STEPPER_ENABLE_2_OSPEEDR_0;
    STEPPER_ENABLE_2_GPIO_PORT->OSPEEDR |=  STEPPER_ENABLE_2_OSPEEDR_1;
    STEPPER_ENABLE_2_GPIO_PORT->PUPDR   &= ~STEPPER_ENABLE_2_PUPD_0;
    STEPPER_ENABLE_2_GPIO_PORT->PUPDR   |=  STEPPER_ENABLE_2_PUPD_1;
    STEPPER_ENABLE_2_GPIO_PORT->ODR     &= ~STEPPER_ENABLE_2_ODR;
#endif

#if(NUMBER_OF_STEPPERS > 3)
    RCC->AHB1ENR |= STEPPER_ENABLE_3_RCC_GPIO_ENABLE;
    STEPPER_ENABLE_3_GPIO_PORT->MODER   &= ~STEPPER_ENABLE_3_MODER_0;
    STEPPER_ENABLE_3_GPIO_PORT->MODER   |=  STEPPER_ENABLE_3_MODER_1;
    STEPPER_ENABLE_3_GPIO_PORT->OTYPER  &= ~STEPPER_ENABLE_3_OTYPER_0;
    STEPPER_ENABLE_3_GPIO_PORT->OTYPER  |=  STEPPER_ENABLE_3_OTYPER_1;
    STEPPER_ENABLE_3_GPIO_PORT->OSPEEDR &= ~STEPPER_ENABLE_3_OSPEEDR_0;
    STEPPER_ENABLE_3_GPIO_PORT->OSPEEDR |=  STEPPER_ENABLE_3_OSPEEDR_1;
    STEPPER_ENABLE_3_GPIO_PORT->PUPDR   &= ~STEPPER_ENABLE_3_PUPD_0;
    STEPPER_ENABLE_3_GPIO_PORT->PUPDR   |=  STEPPER_ENABLE_3_PUPD_1;
    STEPPER_ENABLE_3_GPIO_PORT->ODR     &= ~STEPPER_ENABLE_3_ODR;
#endif

#endif //  HAS_TRINAMIC
}

void hal_setEnable(uint_fast8_t stepper_number, bool on)
{
    switch(stepper_number)
    {
#if(NUMBER_OF_STEPPERS > 0)
    case 0:
        if(true == on)
        {
            STEPPER_ENABLE_0_GPIO_PORT->ODR |= STEPPER_ENABLE_0_ODR;
        }
        else
        {
            STEPPER_ENABLE_0_GPIO_PORT->ODR &= ~STEPPER_ENABLE_0_ODR;
        }
        break;
#endif

#if(NUMBER_OF_STEPPERS > 1)
    case 1:
        if(true == on)
        {
            STEPPER_ENABLE_1_GPIO_PORT->ODR |= STEPPER_ENABLE_1_ODR;
        }
        else
        {
            STEPPER_ENABLE_1_GPIO_PORT->ODR &= ~STEPPER_ENABLE_1_ODR;
        }
        break;
#endif

#if(NUMBER_OF_STEPPERS > 2)
    case 2:
        if(true == on)
        {
            STEPPER_ENABLE_2_GPIO_PORT->ODR |= STEPPER_ENABLE_2_ODR;
        }
        else
        {
            STEPPER_ENABLE_2_GPIO_PORT->ODR &= ~STEPPER_ENABLE_2_ODR;
        }
        break;
#endif

#if(NUMBER_OF_STEPPERS > 3)
    case 3:
        if(true == on)
        {
            STEPPER_ENABLE_3_GPIO_PORT->ODR |= STEPPER_ENABLE_3_ODR;
        }
        else
        {
            STEPPER_ENABLE_3_GPIO_PORT->ODR &= ~STEPPER_ENABLE_3_ODR;
        }
        break;
#endif

    default:
        break;
    }
}

void hal_stepper_set_Output(uint32_t value)
{
    STEPPER_PORT_GPIO_PORT->ODR = value;
}

uint32_t hal_stepper_get_Output(void)
{
    return STEPPER_PORT_GPIO_PORT->ODR;
}
