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
#include "hal_power.h"
#include "board_cfg.h"

void hal_init_power(void)
{
#if (1 == POWER_HAS_HIGH_SWITCH)
    RCC->AHB1ENR |= POWER_HV_RCC_GPIO_ENABLE;
    POWER_HV_GPIO_PORT->MODER &= ~POWER_HV_MODER_0;
    POWER_HV_GPIO_PORT->MODER |=  POWER_HV_MODER_1;
    POWER_HV_GPIO_PORT->OTYPER &= ~POWER_HV_OTYPER_0;
    POWER_HV_GPIO_PORT->OTYPER |=  POWER_HV_OTYPER_1;
    POWER_HV_GPIO_PORT->OSPEEDR &= ~POWER_HV_OSPEEDR_0;
    POWER_HV_GPIO_PORT->OSPEEDR |=  POWER_HV_OSPEEDR_1;
    POWER_HV_GPIO_PORT->PUPDR &= ~POWER_HV_PUPD_0;
    POWER_HV_GPIO_PORT->PUPDR |=  POWER_HV_PUPD_1;
    POWER_HV_GPIO_PORT->ODR &= POWER_HV_ODR;
#endif

#if (1 == POWER_HAS_5V_SWITCH)
    RCC->AHB1ENR |= POWER_5V_RCC_GPIO_ENABLE;
    POWER_5V_GPIO_PORT->MODER &= ~POWER_5V_MODER_0;
    POWER_5V_GPIO_PORT->MODER |=  POWER_5V_MODER_1;
    POWER_5V_GPIO_PORT->OTYPER &= ~POWER_5V_OTYPER_0;
    POWER_5V_GPIO_PORT->OTYPER |=  POWER_5V_OTYPER_1;
    POWER_5V_GPIO_PORT->OSPEEDR &= ~POWER_5V_OSPEEDR_0;
    POWER_5V_GPIO_PORT->OSPEEDR |=  POWER_5V_OSPEEDR_1;
    POWER_5V_GPIO_PORT->PUPDR &= ~POWER_5V_PUPD_0;
    POWER_5V_GPIO_PORT->PUPDR |=  POWER_5V_PUPD_1;
    POWER_5V_GPIO_PORT->ODR &= POWER_5V_ODR;
#endif

#if (1 == POWER_HAS_12V_SWITCH)
    RCC->AHB1ENR |= POWER_12V_RCC_GPIO_ENABLE;
    POWER_12V_GPIO_PORT->MODER &= ~POWER_12V_MODER_0;
    POWER_12V_GPIO_PORT->MODER |=  POWER_12V_MODER_1;
    POWER_12V_GPIO_PORT->OTYPER &= ~POWER_12V_OTYPER_0;
    POWER_12V_GPIO_PORT->OTYPER |=  POWER_12V_OTYPER_1;
    POWER_12V_GPIO_PORT->OSPEEDR &= ~POWER_12V_OSPEEDR_0;
    POWER_12V_GPIO_PORT->OSPEEDR |=  POWER_12V_OSPEEDR_1;
    POWER_12V_GPIO_PORT->PUPDR &= ~POWER_12V_PUPD_0;
    POWER_12V_GPIO_PORT->PUPDR |=  POWER_12V_PUPD_1;
    POWER_12V_GPIO_PORT->ODR &= POWER_12V_ODR;
#endif
}

void hal_power_on_5V(void)
{
#if (1 == POWER_HAS_5V_SWITCH)
    POWER_5V_GPIO_PORT->BSRR_SET = POWER_5V_BSRR;
#endif
}

void hal_power_off_5V(void)
{
#if (1 == POWER_HAS_5V_SWITCH)
    POWER_5V_GPIO_PORT->BSRR_RESET = POWER_5V_BSRR;
#endif
}

void hal_power_on_12V(void)
{
#if (1 == POWER_HAS_12V_SWITCH)
    POWER_12V_GPIO_PORT->BSRR_SET = POWER_12V_BSRR;
#endif
}

void hal_power_off_12V(void)
{
#if (1 == POWER_HAS_12V_SWITCH)
    POWER_12V_GPIO_PORT->BSRR_RESET = POWER_12V_BSRR;
#endif
}

void hal_power_on_HighVoltage(void)
{
#if (1 == POWER_HAS_HIGH_SWITCH)
    POWER_HV_GPIO_PORT->BSRR_SET = POWER_HV_BSRR;
#endif
}

void hal_power_off_HighVoltage(void)
{
#if (1 == POWER_HAS_HIGH_SWITCH)
    POWER_HV_GPIO_PORT->BSRR_RESET = POWER_HV_BSRR;
#endif
}

