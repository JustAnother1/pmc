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

#include <st_util.h>
#include "st_rcc.h"
#include "hal_buzzer.h"
#include "board_cfg.h"

void hal_buzzer_init(void)
{
#if(BUZZER_NUM_PINS > 0)
        RCC->AHB1ENR |= D_OUT_0_RCC_GPIO_ENABLE;
        D_OUT_0_GPIO_PORT->MODER   &= ~D_OUT_0_MODER_0;
        D_OUT_0_GPIO_PORT->MODER   |=  D_OUT_0_MODER_1;
        D_OUT_0_GPIO_PORT->OTYPER  &= ~D_OUT_0_OTYPER_0;
        D_OUT_0_GPIO_PORT->OTYPER  |=  D_OUT_0_OTYPER_1;
        D_OUT_0_GPIO_PORT->OSPEEDR &= ~D_OUT_0_OSPEEDR_0;
        D_OUT_0_GPIO_PORT->OSPEEDR |=  D_OUT_0_OSPEEDR_1;
        D_OUT_0_GPIO_PORT->PUPDR   &= ~D_OUT_0_PUPD_0;
        D_OUT_0_GPIO_PORT->PUPDR   |=  D_OUT_0_PUPD_1;
        D_OUT_0_GPIO_PORT->ODR     &= ~D_OUT_0_ODR;
#endif

}

uint_fast8_t hal_buzzer_get_amount(void)
{
    return BUZZER_NUM_PINS;
}

void hal_buzzer_set_frequency(uint_fast8_t device, uint_fast16_t frequency)
{
    if(device < BUZZER_NUM_PINS)
    {
    	if(0 == frequency)
    	{
    		// Stop Timer -> Low Level
    	}
    	else
    	{
    		// configure Timer
    		// Start Timer
    	}
    }
    // else ignore request for not available device
}

uint_fast8_t hal_buzzer_get_name(uint_fast8_t device, uint8_t *position)
{
    if(device < BUZZER_NUM_PINS)
    {
        switch(device)
        {
        case  0: return copy_string(BUZZER_0_NAME, position);
        default:
            return 0;
        }
    }
    else
    {
        return 0;
    }
}

