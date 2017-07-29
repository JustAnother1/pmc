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
}

void hal_stepper_set_Output(uint32_t value)
{
    STEPPER_PORT_GPIO_PORT->ODR = value;
}

uint32_t hal_stepper_get_Output(void)
{
    return STEPPER_PORT_GPIO_PORT->ODR;
}
