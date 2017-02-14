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
#include "hal_cfg.h"
#include "hal_debug.h"
#include "hal_pwm.h"
#include "hal_time.h"
#include "board_cfg.h"
#include "copy_string.h"

static bool initialized = false;

void hal_pwm_init(void)
{
    if(true == initialized)
    {
        // initialize only once !
        return;
    }
    initialized = true;

    // configure timer for PWM
#if PWM_NUM_PINS > 0
    hal_time_enable_pwm_for(PWM_0_TIMER);
    RCC->AHB1ENR |= PWM_0_RCC_GPIO_ENABLE;
    PWM_0_GPIO_PORT->MODER      &= ~PWM_0_MODER_OFF_0;
    PWM_0_GPIO_PORT->MODER      |=  PWM_0_MODER_OFF_1;
    PWM_0_GPIO_PORT->OTYPER     &= ~PWM_0_OTYPER_0;
    PWM_0_GPIO_PORT->OTYPER     |=  PWM_0_OTYPER_1;
    PWM_0_GPIO_PORT->OSPEEDR    &= ~PWM_0_OSPEEDR_0;
    PWM_0_GPIO_PORT->OSPEEDR    |=  PWM_0_OSPEEDR_1;
    PWM_0_GPIO_PORT->PUPDR      &= ~PWM_0_PUPD_0;
    PWM_0_GPIO_PORT->PUPDR      |=  PWM_0_PUPD_1;
    PWM_0_GPIO_PORT->ODR        &= ~PWM_0_ODR;
    PWM_0_GPIO_PORT->BSRR_RESET  =  PWM_0_BSRR;
#endif
#if PWM_NUM_PINS > 1
    hal_time_enable_pwm_for(PWM_1_TIMER);
    RCC->AHB1ENR |= PWM_1_RCC_GPIO_ENABLE;
    PWM_1_GPIO_PORT->MODER      &= ~PWM_1_MODER_OFF_0;
    PWM_1_GPIO_PORT->MODER      |=  PWM_1_MODER_OFF_1;
    PWM_1_GPIO_PORT->OTYPER     &= ~PWM_1_OTYPER_0;
    PWM_1_GPIO_PORT->OTYPER     |=  PWM_1_OTYPER_1;
    PWM_1_GPIO_PORT->OSPEEDR    &= ~PWM_1_OSPEEDR_0;
    PWM_1_GPIO_PORT->OSPEEDR    |=  PWM_1_OSPEEDR_1;
    PWM_1_GPIO_PORT->PUPDR      &= ~PWM_1_PUPD_0;
    PWM_1_GPIO_PORT->PUPDR      |=  PWM_1_PUPD_1;
    PWM_1_GPIO_PORT->ODR        &= ~PWM_1_ODR;
    PWM_1_GPIO_PORT->BSRR_RESET  =  PWM_1_BSRR;
#endif
#if PWM_NUM_PINS > 2
    hal_time_enable_pwm_for(PWM_2_TIMER);
    RCC->AHB1ENR |= PWM_2_RCC_GPIO_ENABLE;
    PWM_2_GPIO_PORT->MODER      &= ~PWM_2_MODER_OFF_0;
    PWM_2_GPIO_PORT->MODER      |=  PWM_2_MODER_OFF_1;
    PWM_2_GPIO_PORT->OTYPER     &= ~PWM_2_OTYPER_0;
    PWM_2_GPIO_PORT->OTYPER     |=  PWM_2_OTYPER_1;
    PWM_2_GPIO_PORT->OSPEEDR    &= ~PWM_2_OSPEEDR_0;
    PWM_2_GPIO_PORT->OSPEEDR    |=  PWM_2_OSPEEDR_1;
    PWM_2_GPIO_PORT->PUPDR      &= ~PWM_2_PUPD_0;
    PWM_2_GPIO_PORT->PUPDR      |=  PWM_2_PUPD_1;
    PWM_2_GPIO_PORT->ODR        &= ~PWM_2_ODR;
    PWM_2_GPIO_PORT->BSRR_RESET  =  PWM_2_BSRR;
#endif
#if PWM_NUM_PINS > 3
    hal_time_enable_pwm_for(PWM_3_TIMER);
    RCC->AHB1ENR |= PWM_3_RCC_GPIO_ENABLE;
    PWM_3_GPIO_PORT->MODER      &= ~PWM_3_MODER_OFF_0;
    PWM_3_GPIO_PORT->MODER      |=  PWM_3_MODER_OFF_1;
    PWM_3_GPIO_PORT->OTYPER     &= ~PWM_3_OTYPER_0;
    PWM_3_GPIO_PORT->OTYPER     |=  PWM_3_OTYPER_1;
    PWM_3_GPIO_PORT->OSPEEDR    &= ~PWM_3_OSPEEDR_0;
    PWM_3_GPIO_PORT->OSPEEDR    |=  PWM_3_OSPEEDR_1;
    PWM_3_GPIO_PORT->PUPDR      &= ~PWM_3_PUPD_0;
    PWM_3_GPIO_PORT->PUPDR      |=  PWM_3_PUPD_1;
    PWM_3_GPIO_PORT->ODR        &= ~PWM_3_ODR;
    PWM_3_GPIO_PORT->BSRR_RESET  =  PWM_3_BSRR;
#endif
#if PWM_NUM_PINS > 4
    hal_time_enable_pwm_for(PWM_4_TIMER);
    RCC->AHB1ENR |= PWM_4_RCC_GPIO_ENABLE;
    PWM_4_GPIO_PORT->MODER      &= ~PWM_4_MODER_OFF_0;
    PWM_4_GPIO_PORT->MODER      |=  PWM_4_MODER_OFF_1;
    PWM_4_GPIO_PORT->OTYPER     &= ~PWM_4_OTYPER_0;
    PWM_4_GPIO_PORT->OTYPER     |=  PWM_4_OTYPER_1;
    PWM_4_GPIO_PORT->OSPEEDR    &= ~PWM_4_OSPEEDR_0;
    PWM_4_GPIO_PORT->OSPEEDR    |=  PWM_4_OSPEEDR_1;
    PWM_4_GPIO_PORT->PUPDR      &= ~PWM_4_PUPD_0;
    PWM_4_GPIO_PORT->PUPDR      |=  PWM_4_PUPD_1;
    PWM_4_GPIO_PORT->ODR        &= ~PWM_4_ODR;
    PWM_4_GPIO_PORT->BSRR_RESET  =  PWM_4_BSRR;
#endif
#if PWM_NUM_PINS > 5
    hal_time_enable_pwm_for(PWM_5_TIMER);
    RCC->AHB1ENR |= PWM_5_RCC_GPIO_ENABLE;
    PWM_5_GPIO_PORT->MODER      &= ~PWM_5_MODER_OFF_0;
    PWM_5_GPIO_PORT->MODER      |=  PWM_5_MODER_OFF_1;
    PWM_5_GPIO_PORT->OTYPER     &= ~PWM_5_OTYPER_0;
    PWM_5_GPIO_PORT->OTYPER     |=  PWM_5_OTYPER_1;
    PWM_5_GPIO_PORT->OSPEEDR    &= ~PWM_5_OSPEEDR_0;
    PWM_5_GPIO_PORT->OSPEEDR    |=  PWM_5_OSPEEDR_1;
    PWM_5_GPIO_PORT->PUPDR      &= ~PWM_5_PUPD_0;
    PWM_5_GPIO_PORT->PUPDR      |=  PWM_5_PUPD_1;
    PWM_5_GPIO_PORT->ODR        &= ~PWM_5_ODR;
    PWM_5_GPIO_PORT->BSRR_RESET  =  PWM_5_BSRR;
#endif
#if PWM_NUM_PINS > 6
    hal_time_enable_pwm_for(PWM_6_TIMER);
    RCC->AHB1ENR |= PWM_6_RCC_GPIO_ENABLE;
    PWM_6_GPIO_PORT->MODER      &= ~PWM_6_MODER_OFF_0;
    PWM_6_GPIO_PORT->MODER      |=  PWM_6_MODER_OFF_1;
    PWM_6_GPIO_PORT->OTYPER     &= ~PWM_6_OTYPER_0;
    PWM_6_GPIO_PORT->OTYPER     |=  PWM_6_OTYPER_1;
    PWM_6_GPIO_PORT->OSPEEDR    &= ~PWM_6_OSPEEDR_0;
    PWM_6_GPIO_PORT->OSPEEDR    |=  PWM_6_OSPEEDR_1;
    PWM_6_GPIO_PORT->PUPDR      &= ~PWM_6_PUPD_0;
    PWM_6_GPIO_PORT->PUPDR      |=  PWM_6_PUPD_1;
    PWM_6_GPIO_PORT->ODR        &= ~PWM_6_ODR;
    PWM_6_GPIO_PORT->BSRR_RESET  =  PWM_6_BSRR;
#endif
#if PWM_NUM_PINS > 7
    hal_time_enable_pwm_for(PWM_7_TIMER);
    RCC->AHB1ENR |= PWM_7_RCC_GPIO_ENABLE;
    PWM_7_GPIO_PORT->MODER      &= ~PWM_7_MODER_OFF_0;
    PWM_7_GPIO_PORT->MODER      |=  PWM_7_MODER_OFF_1;
    PWM_7_GPIO_PORT->OTYPER     &= ~PWM_7_OTYPER_0;
    PWM_7_GPIO_PORT->OTYPER     |=  PWM_7_OTYPER_1;
    PWM_7_GPIO_PORT->OSPEEDR    &= ~PWM_7_OSPEEDR_0;
    PWM_7_GPIO_PORT->OSPEEDR    |=  PWM_7_OSPEEDR_1;
    PWM_7_GPIO_PORT->PUPDR      &= ~PWM_7_PUPD_0;
    PWM_7_GPIO_PORT->PUPDR      |=  PWM_7_PUPD_1;
    PWM_7_GPIO_PORT->ODR        &= ~PWM_7_ODR;
    PWM_7_GPIO_PORT->BSRR_RESET  =  PWM_7_BSRR;
#endif
#if PWM_NUM_PINS > 8
    hal_time_enable_pwm_for(PWM_8_TIMER);
    RCC->AHB1ENR |= PWM_8_RCC_GPIO_ENABLE;
    PWM_8_GPIO_PORT->MODER      &= ~PWM_8_MODER_OFF_0;
    PWM_8_GPIO_PORT->MODER      |=  PWM_8_MODER_OFF_1;
    PWM_8_GPIO_PORT->OTYPER     &= ~PWM_8_OTYPER_0;
    PWM_8_GPIO_PORT->OTYPER     |=  PWM_8_OTYPER_1;
    PWM_8_GPIO_PORT->OSPEEDR    &= ~PWM_8_OSPEEDR_0;
    PWM_8_GPIO_PORT->OSPEEDR    |=  PWM_8_OSPEEDR_1;
    PWM_8_GPIO_PORT->PUPDR      &= ~PWM_8_PUPD_0;
    PWM_8_GPIO_PORT->PUPDR      |=  PWM_8_PUPD_1;
    PWM_8_GPIO_PORT->ODR        &= ~PWM_8_ODR;
    PWM_8_GPIO_PORT->BSRR_RESET  =  PWM_8_BSRR;
#endif
#if PWM_NUM_PINS > 9
    hal_time_enable_pwm_for(PWM_9_TIMER);
    RCC->AHB1ENR |= PWM_9_RCC_GPIO_ENABLE;
    PWM_9_GPIO_PORT->MODER      &= ~PWM_9_MODER_OFF_0;
    PWM_9_GPIO_PORT->MODER      |=  PWM_9_MODER_OFF_1;
    PWM_9_GPIO_PORT->OTYPER     &= ~PWM_9_OTYPER_0;
    PWM_9_GPIO_PORT->OTYPER     |=  PWM_9_OTYPER_1;
    PWM_9_GPIO_PORT->OSPEEDR    &= ~PWM_9_OSPEEDR_0;
    PWM_9_GPIO_PORT->OSPEEDR    |=  PWM_9_OSPEEDR_1;
    PWM_9_GPIO_PORT->PUPDR      &= ~PWM_9_PUPD_0;
    PWM_9_GPIO_PORT->PUPDR      |=  PWM_9_PUPD_1;
    PWM_9_GPIO_PORT->ODR        &= ~PWM_9_ODR;
    PWM_9_GPIO_PORT->BSRR_RESET  =  PWM_9_BSRR;
#endif
#if PWM_NUM_PINS > 10
    hal_time_enable_pwm_for(PWM_10_TIMER);
    RCC->AHB1ENR |= PWM_10_RCC_GPIO_ENABLE;
    PWM_10_GPIO_PORT->MODER      &= ~PWM_10_MODER_OFF_0;
    PWM_10_GPIO_PORT->MODER      |=  PWM_10_MODER_OFF_1;
    PWM_10_GPIO_PORT->OTYPER     &= ~PWM_10_OTYPER_0;
    PWM_10_GPIO_PORT->OTYPER     |=  PWM_10_OTYPER_1;
    PWM_10_GPIO_PORT->OSPEEDR    &= ~PWM_10_OSPEEDR_0;
    PWM_10_GPIO_PORT->OSPEEDR    |=  PWM_10_OSPEEDR_1;
    PWM_10_GPIO_PORT->PUPDR      &= ~PWM_10_PUPD_0;
    PWM_10_GPIO_PORT->PUPDR      |=  PWM_10_PUPD_1;
    PWM_10_GPIO_PORT->ODR        &= ~PWM_10_ODR;
    PWM_10_GPIO_PORT->BSRR_RESET  =  PWM_10_BSRR;
#endif
#if PWM_NUM_PINS > 11
    hal_time_enable_pwm_for(PWM_11_TIMER);
    RCC->AHB1ENR |= PWM_11_RCC_GPIO_ENABLE;
    PWM_11_GPIO_PORT->MODER      &= ~PWM_11_MODER_OFF_0;
    PWM_11_GPIO_PORT->MODER      |=  PWM_11_MODER_OFF_1;
    PWM_11_GPIO_PORT->OTYPER     &= ~PWM_11_OTYPER_0;
    PWM_11_GPIO_PORT->OTYPER     |=  PWM_11_OTYPER_1;
    PWM_11_GPIO_PORT->OSPEEDR    &= ~PWM_11_OSPEEDR_0;
    PWM_11_GPIO_PORT->OSPEEDR    |=  PWM_11_OSPEEDR_1;
    PWM_11_GPIO_PORT->PUPDR      &= ~PWM_11_PUPD_0;
    PWM_11_GPIO_PORT->PUPDR      |=  PWM_11_PUPD_1;
    PWM_11_GPIO_PORT->ODR        &= ~PWM_11_ODR;
    PWM_11_GPIO_PORT->BSRR_RESET  =  PWM_11_BSRR;
#endif
#if PWM_NUM_PINS > 12
    hal_time_enable_pwm_for(PWM_12_TIMER);
    RCC->AHB1ENR |= PWM_12_RCC_GPIO_ENABLE;
    PWM_12_GPIO_PORT->MODER      &= ~PWM_12_MODER_OFF_0;
    PWM_12_GPIO_PORT->MODER      |=  PWM_12_MODER_OFF_1;
    PWM_12_GPIO_PORT->OTYPER     &= ~PWM_12_OTYPER_0;
    PWM_12_GPIO_PORT->OTYPER     |=  PWM_12_OTYPER_1;
    PWM_12_GPIO_PORT->OSPEEDR    &= ~PWM_12_OSPEEDR_0;
    PWM_12_GPIO_PORT->OSPEEDR    |=  PWM_12_OSPEEDR_1;
    PWM_12_GPIO_PORT->PUPDR      &= ~PWM_12_PUPD_0;
    PWM_12_GPIO_PORT->PUPDR      |=  PWM_12_PUPD_1;
    PWM_12_GPIO_PORT->ODR        &= ~PWM_12_ODR;
    PWM_12_GPIO_PORT->BSRR_RESET  =  PWM_12_BSRR;
#endif
#if PWM_NUM_PINS > 13
    hal_time_enable_pwm_for(PWM_13_TIMER);
    RCC->AHB1ENR |= PWM_13_RCC_GPIO_ENABLE;
    PWM_13_GPIO_PORT->MODER      &= ~PWM_13_MODER_OFF_0;
    PWM_13_GPIO_PORT->MODER      |=  PWM_13_MODER_OFF_1;
    PWM_13_GPIO_PORT->OTYPER     &= ~PWM_13_OTYPER_0;
    PWM_13_GPIO_PORT->OTYPER     |=  PWM_13_OTYPER_1;
    PWM_13_GPIO_PORT->OSPEEDR    &= ~PWM_13_OSPEEDR_0;
    PWM_13_GPIO_PORT->OSPEEDR    |=  PWM_13_OSPEEDR_1;
    PWM_13_GPIO_PORT->PUPDR      &= ~PWM_13_PUPD_0;
    PWM_13_GPIO_PORT->PUPDR      |=  PWM_13_PUPD_1;
    PWM_13_GPIO_PORT->ODR        &= ~PWM_13_ODR;
    PWM_13_GPIO_PORT->BSRR_RESET  =  PWM_13_BSRR;
#endif

}

uint_fast8_t hal_pwm_get_amount(void)
{
    return PWM_NUM_PINS;
}

void hal_pwm_set_on_time(uint_fast8_t device, uint_fast16_t on_time)
{
    if(device < PWM_NUM_PINS)
    {
        if(0 == on_time)
        {
            // switch off PWM Output and set Pin to Low Level
            switch(device)
            {
#if PWM_NUM_PINS > 0
            case 0:
                PWM_0_GPIO_PORT->MODER   &= ~PWM_0_MODER_OFF_0;
                PWM_0_GPIO_PORT->MODER   |=  PWM_0_MODER_OFF_1;
                PWM_0_GPIO_PORT->AFR[0]  |=  PWM_0_AFR_0_1;
                PWM_0_GPIO_PORT->AFR[0]  &= ~PWM_0_AFR_0_0;
                PWM_0_GPIO_PORT->AFR[1]  |=  PWM_0_AFR_1_1;
                PWM_0_GPIO_PORT->AFR[1]  &= ~PWM_0_AFR_1_0;
                hal_time_stop_pwm_for(PWM_0_TIMER, PWM_0_Channel);
                PWM_0_GPIO_PORT->BSRR_RESET = PWM_0_BSRR;
                break;
#endif
#if PWM_NUM_PINS > 1
            case 1:
                PWM_1_GPIO_PORT->MODER   &= ~PWM_1_MODER_OFF_0;
                PWM_1_GPIO_PORT->MODER   |=  PWM_1_MODER_OFF_1;
                PWM_1_GPIO_PORT->AFR[0]  |=  PWM_1_AFR_0_1;
                PWM_1_GPIO_PORT->AFR[0]  &= ~PWM_1_AFR_0_0;
                PWM_1_GPIO_PORT->AFR[1]  |=  PWM_1_AFR_1_1;
                PWM_1_GPIO_PORT->AFR[1]  &= ~PWM_1_AFR_1_0;
                hal_time_stop_pwm_for(PWM_1_TIMER, PWM_1_Channel);
                PWM_1_GPIO_PORT->BSRR_RESET = PWM_1_BSRR;
                break;
#endif
#if PWM_NUM_PINS > 2
            case 2:
                PWM_2_GPIO_PORT->MODER   &= ~PWM_2_MODER_OFF_0;
                PWM_2_GPIO_PORT->MODER   |=  PWM_2_MODER_OFF_1;
                PWM_2_GPIO_PORT->AFR[0]  |=  PWM_2_AFR_0_1;
                PWM_2_GPIO_PORT->AFR[0]  &= ~PWM_2_AFR_0_0;
                PWM_2_GPIO_PORT->AFR[1]  |=  PWM_2_AFR_1_1;
                PWM_2_GPIO_PORT->AFR[1]  &= ~PWM_2_AFR_1_0;
                hal_time_stop_pwm_for(PWM_2_TIMER, PWM_2_Channel);
                PWM_2_GPIO_PORT->BSRR_RESET = PWM_2_BSRR;
                break;
#endif
#if PWM_NUM_PINS > 3
            case 3:
                PWM_3_GPIO_PORT->MODER   &= ~PWM_3_MODER_OFF_0;
                PWM_3_GPIO_PORT->MODER   |=  PWM_3_MODER_OFF_1;
                PWM_3_GPIO_PORT->AFR[0]  |=  PWM_3_AFR_0_1;
                PWM_3_GPIO_PORT->AFR[0]  &= ~PWM_3_AFR_0_0;
                PWM_3_GPIO_PORT->AFR[1]  |=  PWM_3_AFR_1_1;
                PWM_3_GPIO_PORT->AFR[1]  &= ~PWM_3_AFR_1_0;
                hal_time_stop_pwm_for(PWM_3_TIMER, PWM_3_Channel);
                PWM_3_GPIO_PORT->BSRR_RESET = PWM_3_BSRR;
                break;
#endif
#if PWM_NUM_PINS > 4
            case 4:
                PWM_4_GPIO_PORT->MODER   &= ~PWM_4_MODER_OFF_0;
                PWM_4_GPIO_PORT->MODER   |=  PWM_4_MODER_OFF_1;
                PWM_4_GPIO_PORT->AFR[0]  |=  PWM_4_AFR_0_1;
                PWM_4_GPIO_PORT->AFR[0]  &= ~PWM_4_AFR_0_0;
                PWM_4_GPIO_PORT->AFR[1]  |=  PWM_4_AFR_1_1;
                PWM_4_GPIO_PORT->AFR[1]  &= ~PWM_4_AFR_1_0;
                hal_time_stop_pwm_for(PWM_4_TIMER, PWM_4_Channel);
                PWM_4_GPIO_PORT->BSRR_RESET = PWM_4_BSRR;
                break;
#endif
#if PWM_NUM_PINS > 5
            case 5:
                PWM_5_GPIO_PORT->MODER   &= ~PWM_5_MODER_OFF_0;
                PWM_5_GPIO_PORT->MODER   |=  PWM_5_MODER_OFF_1;
                PWM_5_GPIO_PORT->AFR[0]  |=  PWM_5_AFR_0_1;
                PWM_5_GPIO_PORT->AFR[0]  &= ~PWM_5_AFR_0_0;
                PWM_5_GPIO_PORT->AFR[1]  |=  PWM_5_AFR_1_1;
                PWM_5_GPIO_PORT->AFR[1]  &= ~PWM_5_AFR_1_0;
                hal_time_stop_pwm_for(PWM_5_TIMER, PWM_5_Channel);
                PWM_5_GPIO_PORT->BSRR_RESET = PWM_5_BSRR;
                break;
#endif
#if PWM_NUM_PINS > 6
            case 6:
                PWM_6_GPIO_PORT->MODER   &= ~PWM_6_MODER_OFF_0;
                PWM_6_GPIO_PORT->MODER   |=  PWM_6_MODER_OFF_1;
                PWM_6_GPIO_PORT->AFR[0]  |=  PWM_6_AFR_0_1;
                PWM_6_GPIO_PORT->AFR[0]  &= ~PWM_6_AFR_0_0;
                PWM_6_GPIO_PORT->AFR[1]  |=  PWM_6_AFR_1_1;
                PWM_6_GPIO_PORT->AFR[1]  &= ~PWM_6_AFR_1_0;
                hal_time_stop_pwm_for(PWM_6_TIMER, PWM_6_Channel);
                PWM_6_GPIO_PORT->BSRR_RESET = PWM_6_BSRR;
                break;
#endif
#if PWM_NUM_PINS > 7
            case 7:
                PWM_7_GPIO_PORT->MODER   &= ~PWM_7_MODER_OFF_0;
                PWM_7_GPIO_PORT->MODER   |=  PWM_7_MODER_OFF_1;
                PWM_7_GPIO_PORT->AFR[0]  |=  PWM_7_AFR_0_1;
                PWM_7_GPIO_PORT->AFR[0]  &= ~PWM_7_AFR_0_0;
                PWM_7_GPIO_PORT->AFR[1]  |=  PWM_7_AFR_1_1;
                PWM_7_GPIO_PORT->AFR[1]  &= ~PWM_7_AFR_1_0;
                hal_time_stop_pwm_for(PWM_7_TIMER, PWM_7_Channel);
                PWM_7_GPIO_PORT->BSRR_RESET = PWM_7_BSRR;
                break;
#endif
#if PWM_NUM_PINS > 8
            case 8:
                PWM_8_GPIO_PORT->MODER   &= ~PWM_8_MODER_OFF_0;
                PWM_8_GPIO_PORT->MODER   |=  PWM_8_MODER_OFF_1;
                PWM_8_GPIO_PORT->AFR[0]  |=  PWM_8_AFR_0_1;
                PWM_8_GPIO_PORT->AFR[0]  &= ~PWM_8_AFR_0_0;
                PWM_8_GPIO_PORT->AFR[1]  |=  PWM_8_AFR_1_1;
                PWM_8_GPIO_PORT->AFR[1]  &= ~PWM_8_AFR_1_0;
                hal_time_stop_pwm_for(PWM_8_TIMER, PWM_8_Channel);
                PWM_8_GPIO_PORT->BSRR_RESET = PWM_8_BSRR;
                break;
#endif
#if PWM_NUM_PINS > 9
            case 9:
                PWM_9_GPIO_PORT->MODER   &= ~PWM_9_MODER_OFF_0;
                PWM_9_GPIO_PORT->MODER   |=  PWM_9_MODER_OFF_1;
                PWM_9_GPIO_PORT->AFR[0]  |=  PWM_9_AFR_0_1;
                PWM_9_GPIO_PORT->AFR[0]  &= ~PWM_9_AFR_0_0;
                PWM_9_GPIO_PORT->AFR[1]  |=  PWM_9_AFR_1_1;
                PWM_9_GPIO_PORT->AFR[1]  &= ~PWM_9_AFR_1_0;
                hal_time_stop_pwm_for(PWM_9_TIMER, PWM_9_Channel);
                PWM_9_GPIO_PORT->BSRR_RESET = PWM_9_BSRR;
                break;
#endif
#if PWM_NUM_PINS > 10
            case 10:
                PWM_10_GPIO_PORT->MODER   &= ~PWM_10_MODER_OFF_0;
                PWM_10_GPIO_PORT->MODER   |=  PWM_10_MODER_OFF_1;
                PWM_10_GPIO_PORT->AFR[0]  |=  PWM_10_AFR_0_1;
                PWM_10_GPIO_PORT->AFR[0]  &= ~PWM_10_AFR_0_0;
                PWM_10_GPIO_PORT->AFR[1]  |=  PWM_10_AFR_1_1;
                PWM_10_GPIO_PORT->AFR[1]  &= ~PWM_10_AFR_1_0;
                hal_time_stop_pwm_for(PWM_10_TIMER, PWM_10_Channel);
                PWM_10_GPIO_PORT->BSRR_RESET = PWM_10_BSRR;
                break;
#endif
#if PWM_NUM_PINS > 11
            case 11:
                PWM_11_GPIO_PORT->MODER   &= ~PWM_11_MODER_OFF_0;
                PWM_11_GPIO_PORT->MODER   |=  PWM_11_MODER_OFF_1;
                PWM_11_GPIO_PORT->AFR[0]  |=  PWM_11_AFR_0_1;
                PWM_11_GPIO_PORT->AFR[0]  &= ~PWM_11_AFR_0_0;
                PWM_11_GPIO_PORT->AFR[1]  |=  PWM_11_AFR_1_1;
                PWM_11_GPIO_PORT->AFR[1]  &= ~PWM_11_AFR_1_0;
                hal_time_stop_pwm_for(PWM_11_TIMER, PWM_11_Channel);
                PWM_11_GPIO_PORT->BSRR_RESET = PWM_11_BSRR;
                break;
#endif
#if PWM_NUM_PINS > 12
            case 12:
                PWM_12_GPIO_PORT->MODER   &= ~PWM_12_MODER_OFF_0;
                PWM_12_GPIO_PORT->MODER   |=  PWM_12_MODER_OFF_1;
                PWM_12_GPIO_PORT->AFR[0]  |=  PWM_12_AFR_0_1;
                PWM_12_GPIO_PORT->AFR[0]  &= ~PWM_12_AFR_0_0;
                PWM_12_GPIO_PORT->AFR[1]  |=  PWM_12_AFR_1_1;
                PWM_12_GPIO_PORT->AFR[1]  &= ~PWM_12_AFR_1_0;
                hal_time_stop_pwm_for(PWM_12_TIMER, PWM_12_Channel);
                PWM_12_GPIO_PORT->BSRR_RESET = PWM_12_BSRR;
                break;
#endif
#if PWM_NUM_PINS > 13
            case 13:
                PWM_13_GPIO_PORT->MODER   &= ~PWM_13_MODER_OFF_0;
                PWM_13_GPIO_PORT->MODER   |=  PWM_13_MODER_OFF_1;
                PWM_13_GPIO_PORT->AFR[0]  |=  PWM_13_AFR_0_1;
                PWM_13_GPIO_PORT->AFR[0]  &= ~PWM_13_AFR_0_0;
                PWM_13_GPIO_PORT->AFR[1]  |=  PWM_13_AFR_1_1;
                PWM_13_GPIO_PORT->AFR[1]  &= ~PWM_13_AFR_1_0;
                hal_time_stop_pwm_for(PWM_13_TIMER, PWM_13_Channel);
                PWM_13_GPIO_PORT->BSRR_RESET = PWM_13_BSRR
                break;
#endif
            }
        }
        else if(0xffff == on_time)
        {
            // switch off PWM Output and set Pin to High Level
            switch(device)
            {
#if PWM_NUM_PINS > 0
            case 0:
                PWM_0_GPIO_PORT->MODER   &= ~PWM_0_MODER_OFF_0;
                PWM_0_GPIO_PORT->MODER   |=  PWM_0_MODER_OFF_1;
                PWM_0_GPIO_PORT->AFR[0]  |=  PWM_0_AFR_0_1;
                PWM_0_GPIO_PORT->AFR[0]  &= ~PWM_0_AFR_0_0;
                PWM_0_GPIO_PORT->AFR[1]  |=  PWM_0_AFR_1_1;
                PWM_0_GPIO_PORT->AFR[1]  &= ~PWM_0_AFR_1_0;
                hal_time_stop_pwm_for(PWM_0_TIMER, PWM_0_Channel);
                PWM_0_GPIO_PORT->BSRR_SET = PWM_0_BSRR;
                break;
#endif
#if PWM_NUM_PINS > 1
            case 1:
                PWM_1_GPIO_PORT->MODER   &= ~PWM_1_MODER_OFF_0;
                PWM_1_GPIO_PORT->MODER   |=  PWM_1_MODER_OFF_1;
                PWM_1_GPIO_PORT->AFR[0]  |=  PWM_1_AFR_0_1;
                PWM_1_GPIO_PORT->AFR[0]  &= ~PWM_1_AFR_0_0;
                PWM_1_GPIO_PORT->AFR[1]  |=  PWM_1_AFR_1_1;
                PWM_1_GPIO_PORT->AFR[1]  &= ~PWM_1_AFR_1_0;
                hal_time_stop_pwm_for(PWM_1_TIMER, PWM_1_Channel);
                PWM_1_GPIO_PORT->BSRR_SET = PWM_1_BSRR;
                break;
#endif
#if PWM_NUM_PINS > 2
            case 2:
                PWM_2_GPIO_PORT->MODER   &= ~PWM_2_MODER_OFF_0;
                PWM_2_GPIO_PORT->MODER   |=  PWM_2_MODER_OFF_1;
                PWM_2_GPIO_PORT->AFR[0]  |=  PWM_2_AFR_0_1;
                PWM_2_GPIO_PORT->AFR[0]  &= ~PWM_2_AFR_0_0;
                PWM_2_GPIO_PORT->AFR[1]  |=  PWM_2_AFR_1_1;
                PWM_2_GPIO_PORT->AFR[1]  &= ~PWM_2_AFR_1_0;
                hal_time_stop_pwm_for(PWM_2_TIMER, PWM_2_Channel);
                PWM_2_GPIO_PORT->BSRR_SET = PWM_2_BSRR;
                break;
#endif
#if PWM_NUM_PINS > 3
            case 3:
                PWM_3_GPIO_PORT->MODER   &= ~PWM_3_MODER_OFF_0;
                PWM_3_GPIO_PORT->MODER   |=  PWM_3_MODER_OFF_1;
                PWM_3_GPIO_PORT->AFR[0]  |=  PWM_3_AFR_0_1;
                PWM_3_GPIO_PORT->AFR[0]  &= ~PWM_3_AFR_0_0;
                PWM_3_GPIO_PORT->AFR[1]  |=  PWM_3_AFR_1_1;
                PWM_3_GPIO_PORT->AFR[1]  &= ~PWM_3_AFR_1_0;
                hal_time_stop_pwm_for(PWM_3_TIMER, PWM_3_Channel);
                PWM_3_GPIO_PORT->BSRR_SET = PWM_3_BSRR;
                break;
#endif
#if PWM_NUM_PINS > 4
            case 4:
                PWM_4_GPIO_PORT->MODER   &= ~PWM_4_MODER_OFF_0;
                PWM_4_GPIO_PORT->MODER   |=  PWM_4_MODER_OFF_1;
                PWM_4_GPIO_PORT->AFR[0]  |=  PWM_4_AFR_0_1;
                PWM_4_GPIO_PORT->AFR[0]  &= ~PWM_4_AFR_0_0;
                PWM_4_GPIO_PORT->AFR[1]  |=  PWM_4_AFR_1_1;
                PWM_4_GPIO_PORT->AFR[1]  &= ~PWM_4_AFR_1_0;
                hal_time_stop_pwm_for(PWM_4_TIMER, PWM_4_Channel);
                PWM_4_GPIO_PORT->BSRR_SET = PWM_4_BSRR;
                break;
#endif
#if PWM_NUM_PINS > 5
            case 5:
                PWM_5_GPIO_PORT->MODER   &= ~PWM_5_MODER_OFF_0;
                PWM_5_GPIO_PORT->MODER   |=  PWM_5_MODER_OFF_1;
                PWM_5_GPIO_PORT->AFR[0]  |=  PWM_5_AFR_0_1;
                PWM_5_GPIO_PORT->AFR[0]  &= ~PWM_5_AFR_0_0;
                PWM_5_GPIO_PORT->AFR[1]  |=  PWM_5_AFR_1_1;
                PWM_5_GPIO_PORT->AFR[1]  &= ~PWM_5_AFR_1_0;
                hal_time_stop_pwm_for(PWM_5_TIMER, PWM_5_Channel);
                PWM_5_GPIO_PORT->BSRR_SET = PWM_5_BSRR;
                break;
#endif
#if PWM_NUM_PINS > 6
            case 6:
                PWM_6_GPIO_PORT->MODER   &= ~PWM_6_MODER_OFF_0;
                PWM_6_GPIO_PORT->MODER   |=  PWM_6_MODER_OFF_1;
                PWM_6_GPIO_PORT->AFR[0]  |=  PWM_6_AFR_0_1;
                PWM_6_GPIO_PORT->AFR[0]  &= ~PWM_6_AFR_0_0;
                PWM_6_GPIO_PORT->AFR[1]  |=  PWM_6_AFR_1_1;
                PWM_6_GPIO_PORT->AFR[1]  &= ~PWM_6_AFR_1_0;
                hal_time_stop_pwm_for(PWM_6_TIMER, PWM_6_Channel);
                PWM_6_GPIO_PORT->BSRR_SET = PWM_6_BSRR;
                break;
#endif
#if PWM_NUM_PINS > 7
            case 7:
                PWM_7_GPIO_PORT->MODER   &= ~PWM_7_MODER_OFF_0;
                PWM_7_GPIO_PORT->MODER   |=  PWM_7_MODER_OFF_1;
                PWM_7_GPIO_PORT->AFR[0]  |=  PWM_7_AFR_0_1;
                PWM_7_GPIO_PORT->AFR[0]  &= ~PWM_7_AFR_0_0;
                PWM_7_GPIO_PORT->AFR[1]  |=  PWM_7_AFR_1_1;
                PWM_7_GPIO_PORT->AFR[1]  &= ~PWM_7_AFR_1_0;
                hal_time_stop_pwm_for(PWM_7_TIMER, PWM_7_Channel);
                PWM_7_GPIO_PORT->BSRR_SET = PWM_7_BSRR;
                break;
#endif
#if PWM_NUM_PINS > 8
            case 8:
                PWM_8_GPIO_PORT->MODER   &= ~PWM_8_MODER_OFF_0;
                PWM_8_GPIO_PORT->MODER   |=  PWM_8_MODER_OFF_1;
                PWM_8_GPIO_PORT->AFR[0]  |=  PWM_8_AFR_0_1;
                PWM_8_GPIO_PORT->AFR[0]  &= ~PWM_8_AFR_0_0;
                PWM_8_GPIO_PORT->AFR[1]  |=  PWM_8_AFR_1_1;
                PWM_8_GPIO_PORT->AFR[1]  &= ~PWM_8_AFR_1_0;
                hal_time_stop_pwm_for(PWM_8_TIMER, PWM_8_Channel);
                PWM_8_GPIO_PORT->BSRR_SET = PWM_8_BSRR;
                break;
#endif
#if PWM_NUM_PINS > 9
            case 9:
                PWM_9_GPIO_PORT->MODER   &= ~PWM_9_MODER_OFF_0;
                PWM_9_GPIO_PORT->MODER   |=  PWM_9_MODER_OFF_1;
                PWM_9_GPIO_PORT->AFR[0]  |=  PWM_9_AFR_0_1;
                PWM_9_GPIO_PORT->AFR[0]  &= ~PWM_9_AFR_0_0;
                PWM_9_GPIO_PORT->AFR[1]  |=  PWM_9_AFR_1_1;
                PWM_9_GPIO_PORT->AFR[1]  &= ~PWM_9_AFR_1_0;
                hal_time_stop_pwm_for(PWM_9_TIMER, PWM_9_Channel);
                PWM_9_GPIO_PORT->BSRR_SET = PWM_9_BSRR;
                break;
#endif
#if PWM_NUM_PINS > 10
            case 10:
                PWM_10_GPIO_PORT->MODER   &= ~PWM_10_MODER_OFF_0;
                PWM_10_GPIO_PORT->MODER   |=  PWM_10_MODER_OFF_1;
                PWM_10_GPIO_PORT->AFR[0]  |=  PWM_10_AFR_0_1;
                PWM_10_GPIO_PORT->AFR[0]  &= ~PWM_10_AFR_0_0;
                PWM_10_GPIO_PORT->AFR[1]  |=  PWM_10_AFR_1_1;
                PWM_10_GPIO_PORT->AFR[1]  &= ~PWM_10_AFR_1_0;
                hal_time_stop_pwm_for(PWM_10_TIMER, PWM_10_Channel);
                PWM_10_GPIO_PORT->BSRR_SET = PWM_10_BSRR;
                break;
#endif
#if PWM_NUM_PINS > 11
            case 11:
                PWM_11_GPIO_PORT->MODER   &= ~PWM_11_MODER_OFF_0;
                PWM_11_GPIO_PORT->MODER   |=  PWM_11_MODER_OFF_1;
                PWM_11_GPIO_PORT->AFR[0]  |=  PWM_11_AFR_0_1;
                PWM_11_GPIO_PORT->AFR[0]  &= ~PWM_11_AFR_0_0;
                PWM_11_GPIO_PORT->AFR[1]  |=  PWM_11_AFR_1_1;
                PWM_11_GPIO_PORT->AFR[1]  &= ~PWM_11_AFR_1_0;
                hal_time_stop_pwm_for(PWM_11_TIMER, PWM_11_Channel);
                PWM_11_GPIO_PORT->BSRR_SET = PWM_11_BSRR;
                break;
#endif
#if PWM_NUM_PINS > 12
            case 12:
                PWM_12_GPIO_PORT->MODER   &= ~PWM_12_MODER_OFF_0;
                PWM_12_GPIO_PORT->MODER   |=  PWM_12_MODER_OFF_1;
                PWM_12_GPIO_PORT->AFR[0]  |=  PWM_12_AFR_0_1;
                PWM_12_GPIO_PORT->AFR[0]  &= ~PWM_12_AFR_0_0;
                PWM_12_GPIO_PORT->AFR[1]  |=  PWM_12_AFR_1_1;
                PWM_12_GPIO_PORT->AFR[1]  &= ~PWM_12_AFR_1_0;
                hal_time_stop_pwm_for(PWM_12_TIMER, PWM_12_Channel);
                PWM_12_GPIO_PORT->BSRR_SET = PWM_12_BSRR;
                break;
#endif
#if PWM_NUM_PINS > 13
            case 13:
                PWM_13_GPIO_PORT->MODER   &= ~PWM_13_MODER_OFF_0;
                PWM_13_GPIO_PORT->MODER   |=  PWM_13_MODER_OFF_1;
                PWM_13_GPIO_PORT->AFR[0]  |=  PWM_13_AFR_0_1;
                PWM_13_GPIO_PORT->AFR[0]  &= ~PWM_13_AFR_0_0;
                PWM_13_GPIO_PORT->AFR[1]  |=  PWM_13_AFR_1_1;
                PWM_13_GPIO_PORT->AFR[1]  &= ~PWM_13_AFR_1_0;
                hal_time_stop_pwm_for(PWM_13_TIMER, PWM_13_Channel);
                PWM_13_GPIO_PORT->BSRR_SET = PWM_13_BSRR;
                break;
#endif
            }
        }
        else
        {
            // configure PWM
            switch(device)
            {
#if PWM_NUM_PINS > 0
            case 0:
                PWM_0_GPIO_PORT->MODER   &= ~PWM_0_MODER_0;
                PWM_0_GPIO_PORT->MODER   |=  PWM_0_MODER_1;
                PWM_0_GPIO_PORT->AFR[0]  |=  PWM_0_AFR_0_1;
                PWM_0_GPIO_PORT->AFR[0]  &= ~PWM_0_AFR_0_0;
                PWM_0_GPIO_PORT->AFR[1]  |=  PWM_0_AFR_1_1;
                PWM_0_GPIO_PORT->AFR[1]  &= ~PWM_0_AFR_1_0;
                hal_time_set_PWM_for(PWM_0_TIMER, PWM_0_Channel, on_time);
                break;
#endif
#if PWM_NUM_PINS > 1
            case 1:
                PWM_1_GPIO_PORT->MODER   &= ~PWM_1_MODER_0;
                PWM_1_GPIO_PORT->MODER   |=  PWM_1_MODER_1;
                PWM_1_GPIO_PORT->AFR[0]  |=  PWM_1_AFR_0_1;
                PWM_1_GPIO_PORT->AFR[0]  &= ~PWM_1_AFR_0_0;
                PWM_1_GPIO_PORT->AFR[1]  |=  PWM_1_AFR_1_1;
                PWM_1_GPIO_PORT->AFR[1]  &= ~PWM_1_AFR_1_0;
                hal_time_set_PWM_for(PWM_1_TIMER, PWM_1_Channel, on_time);
                break;
#endif
#if PWM_NUM_PINS > 2
            case 2:
                PWM_2_GPIO_PORT->MODER   &= ~PWM_2_MODER_0;
                PWM_2_GPIO_PORT->MODER   |=  PWM_2_MODER_1;
                PWM_2_GPIO_PORT->AFR[0]  |=  PWM_2_AFR_0_1;
                PWM_2_GPIO_PORT->AFR[0]  &= ~PWM_2_AFR_0_0;
                PWM_2_GPIO_PORT->AFR[1]  |=  PWM_2_AFR_1_1;
                PWM_2_GPIO_PORT->AFR[1]  &= ~PWM_2_AFR_1_0;
                hal_time_set_PWM_for(PWM_2_TIMER, PWM_2_Channel, on_time);
                break;
#endif
#if PWM_NUM_PINS > 3
            case 3:
                PWM_3_GPIO_PORT->MODER   &= ~PWM_3_MODER_0;
                PWM_3_GPIO_PORT->MODER   |=  PWM_3_MODER_1;
                PWM_3_GPIO_PORT->AFR[0]  |=  PWM_3_AFR_0_1;
                PWM_3_GPIO_PORT->AFR[0]  &= ~PWM_3_AFR_0_0;
                PWM_3_GPIO_PORT->AFR[1]  |=  PWM_3_AFR_1_1;
                PWM_3_GPIO_PORT->AFR[1]  &= ~PWM_3_AFR_1_0;
                hal_time_set_PWM_for(PWM_3_TIMER, PWM_3_Channel, on_time);
                break;
#endif
#if PWM_NUM_PINS > 4
            case 4:
                PWM_4_GPIO_PORT->MODER   &= ~PWM_4_MODER_0;
                PWM_4_GPIO_PORT->MODER   |=  PWM_4_MODER_1;
                PWM_4_GPIO_PORT->AFR[0]  |=  PWM_4_AFR_0_1;
                PWM_4_GPIO_PORT->AFR[0]  &= ~PWM_4_AFR_0_0;
                PWM_4_GPIO_PORT->AFR[1]  |=  PWM_4_AFR_1_1;
                PWM_4_GPIO_PORT->AFR[1]  &= ~PWM_4_AFR_1_0;
                hal_time_set_PWM_for(PWM_4_TIMER, PWM_4_Channel, on_time);
                break;
#endif
#if PWM_NUM_PINS > 5
            case 5:
                PWM_5_GPIO_PORT->MODER   &= ~PWM_5_MODER_0;
                PWM_5_GPIO_PORT->MODER   |=  PWM_5_MODER_1;
                PWM_5_GPIO_PORT->AFR[0]  |=  PWM_5_AFR_0_1;
                PWM_5_GPIO_PORT->AFR[0]  &= ~PWM_5_AFR_0_0;
                PWM_5_GPIO_PORT->AFR[1]  |=  PWM_5_AFR_1_1;
                PWM_5_GPIO_PORT->AFR[1]  &= ~PWM_5_AFR_1_0;
                hal_time_set_PWM_for(PWM_5_TIMER, PWM_5_Channel, on_time);
                break;
#endif
#if PWM_NUM_PINS > 6
            case 6:
                PWM_6_GPIO_PORT->MODER   &= ~PWM_6_MODER_0;
                PWM_6_GPIO_PORT->MODER   |=  PWM_6_MODER_1;
                PWM_6_GPIO_PORT->AFR[0]  |=  PWM_6_AFR_0_1;
                PWM_6_GPIO_PORT->AFR[0]  &= ~PWM_6_AFR_0_0;
                PWM_6_GPIO_PORT->AFR[1]  |=  PWM_6_AFR_1_1;
                PWM_6_GPIO_PORT->AFR[1]  &= ~PWM_6_AFR_1_0;
                hal_time_set_PWM_for(PWM_6_TIMER, PWM_6_Channel, on_time);
                break;
#endif
#if PWM_NUM_PINS > 7
            case 7:
                PWM_7_GPIO_PORT->MODER   &= ~PWM_7_MODER_0;
                PWM_7_GPIO_PORT->MODER   |=  PWM_7_MODER_1;
                PWM_7_GPIO_PORT->AFR[0]  |=  PWM_7_AFR_0_1;
                PWM_7_GPIO_PORT->AFR[0]  &= ~PWM_7_AFR_0_0;
                PWM_7_GPIO_PORT->AFR[1]  |=  PWM_7_AFR_1_1;
                PWM_7_GPIO_PORT->AFR[1]  &= ~PWM_7_AFR_1_0;
                hal_time_set_PWM_for(PWM_7_TIMER, PWM_7_Channel, on_time);
                break;
#endif
#if PWM_NUM_PINS > 8
            case 8:
                PWM_8_GPIO_PORT->MODER   &= ~PWM_8_MODER_0;
                PWM_8_GPIO_PORT->MODER   |=  PWM_8_MODER_1;
                PWM_8_GPIO_PORT->AFR[0]  |=  PWM_8_AFR_0_1;
                PWM_8_GPIO_PORT->AFR[0]  &= ~PWM_8_AFR_0_0;
                PWM_8_GPIO_PORT->AFR[1]  |=  PWM_8_AFR_1_1;
                PWM_8_GPIO_PORT->AFR[1]  &= ~PWM_8_AFR_1_0;
                hal_time_set_PWM_for(PWM_8_TIMER, PWM_8_Channel, on_time);
                break;
#endif
#if PWM_NUM_PINS > 9
            case 9:
                PWM_9_GPIO_PORT->MODER   &= ~PWM_9_MODER_0;
                PWM_9_GPIO_PORT->MODER   |=  PWM_9_MODER_1;
                PWM_9_GPIO_PORT->AFR[0]  |=  PWM_9_AFR_0_1;
                PWM_9_GPIO_PORT->AFR[0]  &= ~PWM_9_AFR_0_0;
                PWM_9_GPIO_PORT->AFR[1]  |=  PWM_9_AFR_1_1;
                PWM_9_GPIO_PORT->AFR[1]  &= ~PWM_9_AFR_1_0;
                hal_time_set_PWM_for(PWM_9_TIMER, PWM_9_Channel, on_time);
                break;
#endif
#if PWM_NUM_PINS > 10
            case 10:
                PWM_10_GPIO_PORT->MODER   &= ~PWM_10_MODER_0;
                PWM_10_GPIO_PORT->MODER   |=  PWM_10_MODER_1;
                PWM_10_GPIO_PORT->AFR[0]  |=  PWM_10_AFR_0_1;
                PWM_10_GPIO_PORT->AFR[0]  &= ~PWM_10_AFR_0_0;
                PWM_10_GPIO_PORT->AFR[1]  |=  PWM_10_AFR_1_1;
                PWM_10_GPIO_PORT->AFR[1]  &= ~PWM_10_AFR_1_0;
                hal_time_set_PWM_for(PWM_10_TIMER, PWM_10_Channel, on_time);
                break;
#endif
#if PWM_NUM_PINS > 11
            case 11:
                PWM_11_GPIO_PORT->MODER   &= ~PWM_11_MODER_0;
                PWM_11_GPIO_PORT->MODER   |=  PWM_11_MODER_1;
                PWM_11_GPIO_PORT->AFR[0]  |=  PWM_11_AFR_0_1;
                PWM_11_GPIO_PORT->AFR[0]  &= ~PWM_11_AFR_0_0;
                PWM_11_GPIO_PORT->AFR[1]  |=  PWM_11_AFR_1_1;
                PWM_11_GPIO_PORT->AFR[1]  &= ~PWM_11_AFR_1_0;
                hal_time_set_PWM_for(PWM_11_TIMER, PWM_11_Channel, on_time);
                break;
#endif
#if PWM_NUM_PINS > 12
            case 12:
                PWM_12_GPIO_PORT->MODER   &= ~PWM_12_MODER_0;
                PWM_12_GPIO_PORT->MODER   |=  PWM_12_MODER_1;
                PWM_12_GPIO_PORT->AFR[0]  |=  PWM_12_AFR_0_1;
                PWM_12_GPIO_PORT->AFR[0]  &= ~PWM_12_AFR_0_0;
                PWM_12_GPIO_PORT->AFR[1]  |=  PWM_12_AFR_1_1;
                PWM_12_GPIO_PORT->AFR[1]  &= ~PWM_12_AFR_1_0;
                hal_time_set_PWM_for(PWM_12_TIMER, PWM_12_Channel, on_time);
                break;
#endif
#if PWM_NUM_PINS > 13
            case 13:
                PWM_13_GPIO_PORT->MODER   &= ~PWM_13_MODER_0;
                PWM_13_GPIO_PORT->MODER   |=  PWM_13_MODER_1;
                PWM_13_GPIO_PORT->AFR[0]  |=  PWM_13_AFR_0_1;
                PWM_13_GPIO_PORT->AFR[0]  &= ~PWM_13_AFR_0_0;
                PWM_13_GPIO_PORT->AFR[1]  |=  PWM_13_AFR_1_1;
                PWM_13_GPIO_PORT->AFR[1]  &= ~PWM_13_AFR_1_0;
                hal_time_set_PWM_for(PWM_13_TIMER, PWM_13_Channel, on_time);
                break;
#endif
            }
        }
    }
    // else invalid Device
}

uint_fast8_t hal_pwm_get_name(uint_fast8_t device, uint8_t *position, uint_fast8_t max_length)
{
    if(device < PWM_NUM_PINS)
    {
        switch(device)
        {
#if PWM_NUM_PINS > 0
        case  0: return copy_string(PWM_0_NAME, position, max_length);
#endif
#if PWM_NUM_PINS > 1
        case  1: return copy_string(PWM_1_NAME, position, max_length);
#endif
#if PWM_NUM_PINS > 2
        case  2: return copy_string(PWM_2_NAME, position, max_length);
#endif
#if PWM_NUM_PINS > 3
        case  3: return copy_string(PWM_3_NAME, position, max_length);
#endif
#if PWM_NUM_PINS > 4
        case  4: return copy_string(PWM_4_NAME, position, max_length);
#endif
#if PWM_NUM_PINS > 5
        case  5: return copy_string(PWM_5_NAME, position, max_length);
#endif
#if PWM_NUM_PINS > 6
        case  6: return copy_string(PWM_6_NAME, position, max_length);
#endif
#if PWM_NUM_PINS > 7
        case  7: return copy_string(PWM_7_NAME, position, max_length);
#endif
#if PWM_NUM_PINS > 8
        case  8: return copy_string(PWM_8_NAME, position, max_length);
#endif
#if PWM_NUM_PINS > 9
        case  9: return copy_string(PWM_9_NAME, position, max_length);
#endif
#if PWM_NUM_PINS > 10
        case 10: return copy_string(PWM_10_NAME, position, max_length);
#endif
#if PWM_NUM_PINS > 11
        case 11: return copy_string(PWM_11_NAME, position, max_length);
#endif
#if PWM_NUM_PINS > 12
        case 12: return copy_string(PWM_12_NAME, position, max_length);
#endif
#if PWM_NUM_PINS > 13
        case 13: return copy_string(PWM_13_NAME, position, max_length);
#endif
        default:
            return 0;
        }
    }
    else
    {
        return 0;
    }
}

