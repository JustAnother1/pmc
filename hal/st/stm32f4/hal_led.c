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
#include "hal_cfg.h"

typedef struct {
    GPIO_TypeDef * port;
    uint16_t mask;
    bool is_on;
}led_device_typ;

static volatile led_device_typ devices[MAX_LED];

void hal_led_init(void)
{
// Debug LED
    RCC->AHB1ENR |= DEBUG_LED_RCC_GPIO_ENABLE;
    // LEDS are general purpose outputs
    DEBUG_LED_GPIO_PORT->MODER &= ~DEBUG_LED_MODER_0;
    DEBUG_LED_GPIO_PORT->MODER |=  DEBUG_LED_MODER_1;
    // LEDS are push pull
    DEBUG_LED_GPIO_PORT->OTYPER &= ~DEBUG_LED_OTYPER_0;
    DEBUG_LED_GPIO_PORT->OTYPER |=  DEBUG_LED_OTYPER_1;
    // LEDS are low speed (= max 2MHz) but high current
    DEBUG_LED_GPIO_PORT->OSPEEDR &= ~DEBUG_LED_OSPEEDR_0;
    DEBUG_LED_GPIO_PORT->OSPEEDR |=  DEBUG_LED_OSPEEDR_1;
    // No pull ups or pull downs
    DEBUG_LED_GPIO_PORT->PUPDR &= ~DEBUG_LED_PUPD_0;
    DEBUG_LED_GPIO_PORT->PUPDR |=  DEBUG_LED_PUPD_1;
    // start with output = 0
    DEBUG_LED_GPIO_PORT->ODR &= DEBUG_LED_ODR;
    devices[DEBUG_LED].port = DEBUG_LED_GPIO_PORT;
    devices[DEBUG_LED].mask = DEBUG_LED_BSRR;
    devices[DEBUG_LED].is_on = false;

// Error LED
    RCC->AHB1ENR |= ERROR_LED_RCC_GPIO_ENABLE;
    // LEDS are general purpose outputs
    ERROR_LED_GPIO_PORT->MODER &= ~ERROR_LED_MODER_0;
    ERROR_LED_GPIO_PORT->MODER |=  ERROR_LED_MODER_1;
    // LEDS are push pull
    ERROR_LED_GPIO_PORT->OTYPER &= ~ERROR_LED_OTYPER_0;
    ERROR_LED_GPIO_PORT->OTYPER |=  ERROR_LED_OTYPER_1;
    // LEDS are high speed
    ERROR_LED_GPIO_PORT->OSPEEDR &= ~ERROR_LED_OSPEEDR_0;
    ERROR_LED_GPIO_PORT->OSPEEDR |=  ERROR_LED_OSPEEDR_1;
    // No pull ups or pull downs
    ERROR_LED_GPIO_PORT->PUPDR &= ~ERROR_LED_PUPD_0;
    ERROR_LED_GPIO_PORT->PUPDR |=  ERROR_LED_PUPD_1;
    // start with output = 0
    ERROR_LED_GPIO_PORT->ODR &= ERROR_LED_ODR;
    devices[ERROR_LED].port = ERROR_LED_GPIO_PORT;
    devices[ERROR_LED].mask = ERROR_LED_BSRR;
    devices[ERROR_LED].is_on = false;
}

void hal_led_toggle_led(uint_fast8_t device)
{
    if(device < MAX_LED)
    {
        if(true == devices[device].is_on)
        {
            devices[device].port->BSRR_RESET = devices[device].mask;
            devices[device].is_on = false;
        }
        else
        {
            devices[device].port->BSRR_SET = devices[device].mask;
            devices[device].is_on = true;
        }
    }
    // else invalid LED specified
}

void hal_led_set_led(uint_fast8_t device, bool on)
{
    if(device < MAX_LED)
    {
        if(false == on)
        {
            devices[device].port->BSRR_RESET = devices[device].mask;
            devices[device].is_on = false;
        }
        else
        {
            devices[device].port->BSRR_SET = devices[device].mask;
            devices[device].is_on = true;
        }
    }
    // else invalid LED specified
}

