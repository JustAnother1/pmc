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
#include "rcc.h"
#include "gpio.h"
#include "board_cfg.h"

void hal_stepper_port_init(void)
{
    RCC->AHB1ENR |= STEPPER_PORT_RCC_GPIO_ENABLE;
    STEPPER_PORT_GPIO_PORT->MODER   = STEPPER_PORT_MODER;
    STEPPER_PORT_GPIO_PORT->OTYPER  = STEPPER_PORT_OTYPER;
    STEPPER_PORT_GPIO_PORT->OSPEEDR = STEPPER_PORT_OSPEEDR;
    STEPPER_PORT_GPIO_PORT->PUPDR   = STEPPER_PORT_PUPD;
    STEPPER_PORT_GPIO_PORT->ODR     = STEPPER_PORT_ODR;
}

inline void hal_stepper_set_Output(uint32_t value)
{
    STEPPER_PORT_GPIO_PORT->ODR = value;
}
