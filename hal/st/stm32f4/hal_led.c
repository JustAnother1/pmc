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

#include "hal_led.h"
#include "gpio.h"

static bool debugLedIsOn = false;

void hal_led_init(void)
{
    // LEDS are outputs
    GPIOD->MODER = GPIO_MODER_MODER15_0
                 + GPIO_MODER_MODER14_0
                 + GPIO_MODER_MODER13_0
                 + GPIO_MODER_MODER12_0;
    // LEDS are push pull
    GPIOD->OTYPER = (GPIOD->OTYPER & 0xffff0000);
    // LEDS are high speed
    GPIOD->OSPEEDR = GPIO_OSPEEDER_OSPEEDR15_1
                   + GPIO_OSPEEDER_OSPEEDR15_0
                   + GPIO_OSPEEDER_OSPEEDR14_1
                   + GPIO_OSPEEDER_OSPEEDR14_0
                   + GPIO_OSPEEDER_OSPEEDR13_1
                   + GPIO_OSPEEDER_OSPEEDR13_0
                   + GPIO_OSPEEDER_OSPEEDR12_1
                   + GPIO_OSPEEDER_OSPEEDR12_0;
    // No pull ups or pull downs
    GPIOD->PUPDR = 0;
    // start with output = 0
    GPIOD->ODR = 0;
}

void hal_led_toggle_debug_led(void)
{
    if(true == debugLedIsOn)
    {
        GPIOD->ODR = (GPIOD->ODR & ~GPIO_ODR_ODR_12);
        debugLedIsOn = false;
    }
    else
    {
        GPIOD->ODR = (GPIOD->ODR |GPIO_ODR_ODR_12);
        debugLedIsOn = true;
    }
}

void hal_led_set_error_led(bool on)
{
    if(true == on)
    {
        // led on
        GPIOD->ODR = (GPIOD->ODR |GPIO_ODR_ODR_14);
    }
    else
    {
        // led off
        GPIOD->ODR = (GPIOD->ODR & ~GPIO_ODR_ODR_14);
    }
}

