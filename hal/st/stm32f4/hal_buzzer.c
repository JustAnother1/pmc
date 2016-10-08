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
#include <stdio.h>
#include <st_common.h>
#include <st_util.h>
#include "st_rcc.h"
#include "hal_buzzer.h"
#include "hal_time.h"
#include "board_cfg.h"
#include "hal_cfg.h"

#define TIMER_FREQUENCY 500000

void hal_buzzer_init(void)
{
#if(BUZZER_NUM_PINS > 0)
        RCC->AHB1ENR |= D_OUT_0_RCC_GPIO_ENABLE;
        BUZZER_0_GPIO_PORT->MODER   &= ~BUZZER_0_MODER_0;
        BUZZER_0_GPIO_PORT->MODER   |=  BUZZER_0_MODER_1;
        BUZZER_0_GPIO_PORT->OTYPER  &= ~BUZZER_0_OTYPER_0;
        BUZZER_0_GPIO_PORT->OTYPER  |=  BUZZER_0_OTYPER_1;
        BUZZER_0_GPIO_PORT->OSPEEDR &= ~BUZZER_0_OSPEEDR_0;
        BUZZER_0_GPIO_PORT->OSPEEDR |=  BUZZER_0_OSPEEDR_1;
        BUZZER_0_GPIO_PORT->PUPDR   &= ~BUZZER_0_PUPD_0;
        BUZZER_0_GPIO_PORT->PUPDR   |=  BUZZER_0_PUPD_1;
        BUZZER_0_GPIO_PORT->ODR     &= ~BUZZER_0_ODR;
        BUZZER_0_GPIO_PORT->AFR[0]  |=  BUZZER_0_AFR_0_1;
        BUZZER_0_GPIO_PORT->AFR[0]  &= ~BUZZER_0_AFR_0_0;
        BUZZER_0_GPIO_PORT->AFR[1]  |=  BUZZER_0_AFR_1_1;
        BUZZER_0_GPIO_PORT->AFR[1]  &= ~BUZZER_0_AFR_1_0;
#endif

}

void curTest(void)
{
	hal_buzzer_set_frequency(1, 500);
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
    		hal_time_stop_timer(BUZZER_TIMER);
    	}
    	else
    	{
    		// calculate Reload Value
    		int reload = TIMER_FREQUENCY/frequency;
    		if(reload > 0xffff)
    		{
    			reload = 0xffff;
    		}
    		// Start Timer
    		hal_time_start_timer(BUZZER_TIMER, TIMER_FREQUENCY, reload, NULL);
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

