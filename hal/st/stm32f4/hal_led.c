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

#include "rcc.h"
#include "gpio.h"
#include "board_cfg.h"
#include "hal_led.h"

static bool debugLedIsOn = false;

void hal_led_init(void)
{
// Debug LED
    RCC->AHB1ENR |= DEBUG_LED_RCC_GPIO_ENABLE;
    // LEDS are general purpose outputs
    DEBUG_LED_GPIO_PORT->MODER |= DEBUG_LED_MODER;
    // LEDS are push pull
    DEBUG_LED_GPIO_PORT->OTYPER &= ~DEBUG_LED_OTYPER;
    // LEDS are low speed (= max 2MHz) but high current
    DEBUG_LED_GPIO_PORT->OSPEEDR &= ~DEBUG_LED_OSPEEDR;
    // No pull ups or pull downs
    DEBUG_LED_GPIO_PORT->PUPDR &= ~ DEBUG_LED_PUPD;
    // start with output = 0
    DEBUG_LED_GPIO_PORT->ODR &= DEBUG_LED_ODR;

// Error LED
    RCC->AHB1ENR |= ERROR_LED_RCC_GPIO_ENABLE;
    // LEDS are general purpose outputs
    ERROR_LED_GPIO_PORT->MODER |= ERROR_LED_MODER;
    // LEDS are push pull
    ERROR_LED_GPIO_PORT->OTYPER &= ~ERROR_LED_OTYPER;
    // LEDS are high speed
    ERROR_LED_GPIO_PORT->OSPEEDR &= ~ERROR_LED_OSPEEDR;
    // No pull ups or pull downs
    ERROR_LED_GPIO_PORT->PUPDR &= ~ ERROR_LED_PUPD;
    // start with output = 0
    ERROR_LED_GPIO_PORT->ODR &= ERROR_LED_ODR;
}

void hal_led_toggle_debug_led(void)
{
    if(true == debugLedIsOn)
    {
        DEBUG_LED_GPIO_PORT->BSRR_RESET = DEBUG_LED_BSRR;
        debugLedIsOn = false;
    }
    else
    {
        DEBUG_LED_GPIO_PORT->BSRR_SET = DEBUG_LED_BSRR;
        debugLedIsOn = true;
    }
}

void hal_led_set_error_led(bool on)
{
    if(true == on)
    {
        // led on
        ERROR_LED_GPIO_PORT->BSRR_SET = ERROR_LED_BSRR;
    }
    else
    {
        // led off
        ERROR_LED_GPIO_PORT->BSRR_RESET = ERROR_LED_BSRR;
    }
}

