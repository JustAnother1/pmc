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
#include "board_cfg.h"
#include "hal_led.h"
#include "hal_cfg.h"

typedef struct {
    GPIO_TypeDef * port;
    uint16_t mask;
    bool is_on;
}led_device_typ;

static volatile led_device_typ devices[NUMBER_OF_LED];
static bool initialized = false;

static void hal_led_init(void);
static void hal_led_toggle_led(uint_fast8_t device);
static void hal_led_set_led(uint_fast8_t device, bool on);


// Implementation of hal_led_api


void hal_init_leds(void)
{
    hal_led_init();
}

#ifdef BOARD_HAS_DEBUG_LED
void hal_toggle_debug_led(void)
{
    hal_led_toggle_led(DEBUG_LED);
}

void hal_set_debug_led(bool on)
{
    hal_led_set_led(DEBUG_LED, on);
}
#endif

#ifdef BOARD_HAS_ERROR_LED
void hal_toggle_error_led(void)
{
    hal_led_toggle_led(ERROR_LED);
}

void hal_set_error_led(bool on)
{
    hal_led_set_led(ERROR_LED, on);
}
#endif

#ifdef BOARD_HAS_ISR_1_LED
void hal_toggle_isr1_led(void)
{
    hal_led_toggle_led(ISR_1_LED);
}

void hal_set_isr1_led(bool on)
{
    hal_led_set_led(ISR_1_LED, on);
}
#endif

#ifdef BOARD_HAS_ISR_2_LED
void hal_toggle_isr2_led(void)
{
    hal_led_toggle_led(ISR_2_LED);
}

void hal_set_isr2_led(bool on)
{
    hal_led_set_led(ISR_2_LED, on);
}
#endif

// end of hal_led_api


void hal_led_init(void)
{
    if(true == initialized)
    {
        // initialize only once !
        return;
    }
    initialized = true;

#if NUMBER_OF_LED > 0
    RCC->AHB1ENR |= LED_0_RCC_GPIO_ENABLE;
    // LEDS are general purpose outputs
    LED_0_GPIO_PORT->MODER &= ~LED_0_MODER_0;
    LED_0_GPIO_PORT->MODER |=  LED_0_MODER_1;
    // LEDS are push pull
    LED_0_GPIO_PORT->OTYPER &= ~LED_0_OTYPER_0;
    LED_0_GPIO_PORT->OTYPER |=  LED_0_OTYPER_1;
    // LEDS are low speed (= max 2MHz) but high current
    LED_0_GPIO_PORT->OSPEEDR &= ~LED_0_OSPEEDR_0;
    LED_0_GPIO_PORT->OSPEEDR |=  LED_0_OSPEEDR_1;
    // No pull ups or pull downs
    LED_0_GPIO_PORT->PUPDR &= ~LED_0_PUPD_0;
    LED_0_GPIO_PORT->PUPDR |=  LED_0_PUPD_1;
    // start with output = 0
    LED_0_GPIO_PORT->ODR &= LED_0_ODR;
    devices[0].port = LED_0_GPIO_PORT;
    devices[0].mask = LED_0_BSRR;
    devices[0].is_on = false;
#endif

#if NUMBER_OF_LED > 1
    RCC->AHB1ENR |= LED_1_RCC_GPIO_ENABLE;
    // LEDS are general purpose outputs
    LED_1_GPIO_PORT->MODER &= ~LED_1_MODER_0;
    LED_1_GPIO_PORT->MODER |=  LED_1_MODER_1;
    // LEDS are push pull
    LED_1_GPIO_PORT->OTYPER &= ~LED_1_OTYPER_0;
    LED_1_GPIO_PORT->OTYPER |=  LED_1_OTYPER_1;
    // LEDS are low speed (= max 2MHz) but high current
    LED_1_GPIO_PORT->OSPEEDR &= ~LED_1_OSPEEDR_0;
    LED_1_GPIO_PORT->OSPEEDR |=  LED_1_OSPEEDR_1;
    // No pull ups or pull downs
    LED_1_GPIO_PORT->PUPDR &= ~LED_1_PUPD_0;
    LED_1_GPIO_PORT->PUPDR |=  LED_1_PUPD_1;
    // start with output = 0
    LED_1_GPIO_PORT->ODR &= LED_1_ODR;
    devices[1].port = LED_1_GPIO_PORT;
    devices[1].mask = LED_1_BSRR;
    devices[1].is_on = false;
#endif

#if NUMBER_OF_LED > 2
    RCC->AHB1ENR |= LED_2_RCC_GPIO_ENABLE;
    // LEDS are general purpose outputs
    LED_2_GPIO_PORT->MODER &= ~LED_2_MODER_0;
    LED_2_GPIO_PORT->MODER |=  LED_2_MODER_1;
    // LEDS are push pull
    LED_2_GPIO_PORT->OTYPER &= ~LED_2_OTYPER_0;
    LED_2_GPIO_PORT->OTYPER |=  LED_2_OTYPER_1;
    // LEDS are low speed (= max 2MHz) but high current
    LED_2_GPIO_PORT->OSPEEDR &= ~LED_2_OSPEEDR_0;
    LED_2_GPIO_PORT->OSPEEDR |=  LED_2_OSPEEDR_1;
    // No pull ups or pull downs
    LED_2_GPIO_PORT->PUPDR &= ~LED_2_PUPD_0;
    LED_2_GPIO_PORT->PUPDR |=  LED_2_PUPD_1;
    // start with output = 0
    LED_2_GPIO_PORT->ODR &= LED_2_ODR;
    devices[2].port = LED_2_GPIO_PORT;
    devices[2].mask = LED_2_BSRR;
    devices[2].is_on = false;
#endif

#if NUMBER_OF_LED > 3
    RCC->AHB1ENR |= LED_3_RCC_GPIO_ENABLE;
    // LEDS are general purpose outputs
    LED_3_GPIO_PORT->MODER &= ~LED_3_MODER_0;
    LED_3_GPIO_PORT->MODER |=  LED_3_MODER_1;
    // LEDS are push pull
    LED_3_GPIO_PORT->OTYPER &= ~LED_3_OTYPER_0;
    LED_3_GPIO_PORT->OTYPER |=  LED_3_OTYPER_1;
    // LEDS are low speed (= max 2MHz) but high current
    LED_3_GPIO_PORT->OSPEEDR &= ~LED_3_OSPEEDR_0;
    LED_3_GPIO_PORT->OSPEEDR |=  LED_3_OSPEEDR_1;
    // No pull ups or pull downs
    LED_3_GPIO_PORT->PUPDR &= ~LED_3_PUPD_0;
    LED_3_GPIO_PORT->PUPDR |=  LED_3_PUPD_1;
    // start with output = 0
    LED_3_GPIO_PORT->ODR &= LED_3_ODR;
    devices[3].port = LED_3_GPIO_PORT;
    devices[3].mask = LED_3_BSRR;
    devices[3].is_on = false;
#endif
}

void hal_led_toggle_led(uint_fast8_t device)
{
    if(device < NUMBER_OF_LED)
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
    if(device < NUMBER_OF_LED)
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

