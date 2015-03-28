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

#include <st_common.h>
#include <st_rcc.h>
#include <st_timer.h>
#include "hal_cpu.h"
#include "hal_time.h"
#include "hal_led.h"
#include "stddef.h"
#include "board_cfg.h"

static void hal_time_ISR(void);
static TIM_TypeDef* get_timer_register_for(uint_fast8_t device);
static void enable_clock_for_timer(uint_fast8_t device);
static void disable_clock_for_timer(uint_fast8_t device);
static void error_isr_on_stopped_timer(void);

static volatile uint32_t now = 0;
static volatile TimerFkt tim_1_isr;
static volatile TimerFkt tim_2_isr;
static volatile TimerFkt tim_3_isr;
static volatile TimerFkt tim_4_isr;
static volatile TimerFkt tim_5_isr;
static volatile TimerFkt tim_6_isr;
static volatile TimerFkt tim_7_isr;
static volatile TimerFkt tim_8_isr;
static volatile TimerFkt tim_9_isr;
static volatile TimerFkt tim_10_isr;
static volatile TimerFkt tim_11_isr;
static volatile TimerFkt tim_12_isr;
static volatile TimerFkt tim_13_isr;
static volatile TimerFkt tim_14_isr;

void hal_time_init(void)
{
    tim_1_isr =  &error_isr_on_stopped_timer;
    tim_2_isr =  &error_isr_on_stopped_timer;
    tim_3_isr =  &error_isr_on_stopped_timer;
    tim_4_isr =  &error_isr_on_stopped_timer;
    tim_5_isr =  &error_isr_on_stopped_timer;
    tim_6_isr =  &error_isr_on_stopped_timer;
    tim_7_isr =  &error_isr_on_stopped_timer;
    tim_8_isr =  &error_isr_on_stopped_timer;
    tim_9_isr =  &error_isr_on_stopped_timer;
    tim_10_isr =  &error_isr_on_stopped_timer;
    tim_11_isr =  &error_isr_on_stopped_timer;
    tim_12_isr =  &error_isr_on_stopped_timer;
    tim_13_isr =  &error_isr_on_stopped_timer;
    tim_14_isr =  &error_isr_on_stopped_timer;
    hal_cpu_add_ms_tick_function(&hal_time_ISR);
}

static void hal_time_ISR(void)
{
    now++;
}

uint32_t hal_time_get_ms_tick(void)
{
    return now;
}

static void error_isr_on_stopped_timer(void)
{
    // TODO ???
}

void hal_time_ms_sleep(uint_fast32_t ms)
{
    uint32_t tickend = hal_time_get_ms_tick() + ms;
    uint32_t curtick = hal_time_get_ms_tick();
    while(curtick < tickend)
    {
        curtick = hal_time_get_ms_tick();
    }
}

static TIM_TypeDef* get_timer_register_for(uint_fast8_t device)
{
    switch(device)
    {
    case  1:return TIM1;
    case  2:return TIM2;
    case  3:return TIM3;
    case  4:return TIM4;
    case  5:return TIM5;
    case  6:return TIM6;
    case  7:return TIM7;
    case  8:return TIM8;
    case  9:return TIM9;
    case 10:return TIM10;
    case 11:return TIM11;
    case 12:return TIM12;
    case 13:return TIM13;
    case 14:return TIM14;
    default: return NULL;
    }
}

static void enable_clock_for_timer(uint_fast8_t device)
{
    switch(device)
    {
    case  1: RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;  break;
    case  2: RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;  break;
    case  3: RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;  break;
    case  4: RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;  break;
    case  5: RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;  break;
    case  6: RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;  break;
    case  7: RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;  break;
    case  8: RCC->APB2ENR |= RCC_APB2ENR_TIM8EN;  break;
    case  9: RCC->APB2ENR |= RCC_APB2ENR_TIM9EN;  break;
    case 10: RCC->APB2ENR |= RCC_APB2ENR_TIM10EN; break;
    case 11: RCC->APB2ENR |= RCC_APB2ENR_TIM11EN; break;
    case 12: RCC->APB1ENR |= RCC_APB1ENR_TIM12EN; break;
    case 13: RCC->APB1ENR |= RCC_APB1ENR_TIM13EN; break;
    case 14: RCC->APB1ENR |= RCC_APB1ENR_TIM14EN; break;
    default: return;
    }
}

static void disable_clock_for_timer(uint_fast8_t device)
{
    switch(device)
    {
    case  1: RCC->APB2ENR &= ~RCC_APB2ENR_TIM1EN;  break;
    case  2: RCC->APB1ENR &= ~RCC_APB1ENR_TIM2EN;  break;
    case  3: RCC->APB1ENR &= ~RCC_APB1ENR_TIM3EN;  break;
    case  4: RCC->APB1ENR &= ~RCC_APB1ENR_TIM4EN;  break;
    case  5: RCC->APB1ENR &= ~RCC_APB1ENR_TIM5EN;  break;
    case  6: RCC->APB1ENR &= ~RCC_APB1ENR_TIM6EN;  break;
    case  7: RCC->APB1ENR &= ~RCC_APB1ENR_TIM7EN;  break;
    case  8: RCC->APB2ENR &= ~RCC_APB2ENR_TIM8EN;  break;
    case  9: RCC->APB2ENR &= ~RCC_APB2ENR_TIM9EN;  break;
    case 10: RCC->APB2ENR &= ~RCC_APB2ENR_TIM10EN; break;
    case 11: RCC->APB2ENR &= ~RCC_APB2ENR_TIM11EN; break;
    case 12: RCC->APB1ENR &= ~RCC_APB1ENR_TIM12EN; break;
    case 13: RCC->APB1ENR &= ~RCC_APB1ENR_TIM13EN; break;
    case 14: RCC->APB1ENR &= ~RCC_APB1ENR_TIM14EN; break;
    default: return;
    }
}

static void set_irq_priority(uint_fast8_t device)
{
    switch(device)
    {
    case 1:
        NVIC_SetPriority(TIM_1_IRQ_NUMBER, TIM_1_IRQ_PRIORITY);
        NVIC_EnableIRQ(TIM_1_IRQ_NUMBER);
        break;
    case 2:
        NVIC_SetPriority(TIM_2_IRQ_NUMBER, TIM_2_IRQ_PRIORITY);
        NVIC_EnableIRQ(TIM_2_IRQ_NUMBER);
        break;
    case 3:
        NVIC_SetPriority(TIM_3_IRQ_NUMBER, TIM_3_IRQ_PRIORITY);
        NVIC_EnableIRQ(TIM_3_IRQ_NUMBER);
        break;
    case 4:
        NVIC_SetPriority(TIM_4_IRQ_NUMBER, TIM_4_IRQ_PRIORITY);
        NVIC_EnableIRQ(TIM_4_IRQ_NUMBER);
        break;
    case 5:
        NVIC_SetPriority(TIM_5_IRQ_NUMBER, TIM_5_IRQ_PRIORITY);
        NVIC_EnableIRQ(TIM_5_IRQ_NUMBER);
        break;
    case 6:
        NVIC_SetPriority(TIM_6_IRQ_NUMBER, TIM_6_IRQ_PRIORITY);
        NVIC_EnableIRQ(TIM_6_IRQ_NUMBER);
        break;
    case 7:
        NVIC_SetPriority(TIM_7_IRQ_NUMBER, TIM_7_IRQ_PRIORITY);
        NVIC_EnableIRQ(TIM_7_IRQ_NUMBER);
        break;
    case 8:
        NVIC_SetPriority(TIM_8_IRQ_NUMBER, TIM_8_IRQ_PRIORITY);
        NVIC_EnableIRQ(TIM_8_IRQ_NUMBER);
        break;
    case 9:
        NVIC_SetPriority(TIM_9_IRQ_NUMBER, TIM_9_IRQ_PRIORITY);
        NVIC_EnableIRQ(TIM_9_IRQ_NUMBER);
        break;
    case 10:
        NVIC_SetPriority(TIM_10_IRQ_NUMBER, TIM_10_IRQ_PRIORITY);
        NVIC_EnableIRQ(TIM_10_IRQ_NUMBER);
        break;
    case 11:
        NVIC_SetPriority(TIM_11_IRQ_NUMBER, TIM_11_IRQ_PRIORITY);
        NVIC_EnableIRQ(TIM_11_IRQ_NUMBER);
        break;
    case 12:
        NVIC_SetPriority(TIM_12_IRQ_NUMBER, TIM_12_IRQ_PRIORITY);
        NVIC_EnableIRQ(TIM_12_IRQ_NUMBER);
        break;
    case 13:
        NVIC_SetPriority(TIM_13_IRQ_NUMBER, TIM_13_IRQ_PRIORITY);
        NVIC_EnableIRQ(TIM_13_IRQ_NUMBER);
        break;
    case 14:
        NVIC_SetPriority(TIM_14_IRQ_NUMBER, TIM_14_IRQ_PRIORITY);
        NVIC_EnableIRQ(TIM_14_IRQ_NUMBER);
        break;
    }
}

void TIM1_CC_IRQHandler(void)
{
    tim_1_isr();
    TIM1->SR &= ~TIM_SR_UIF;
}

void TIM2_IRQHandler(void)
{
    tim_2_isr();
    TIM2->SR &= ~TIM_SR_UIF;
}

void TIM3_IRQHandler(void)
{
    tim_3_isr();
    TIM3->SR &= ~TIM_SR_UIF;
}

void TIM4_IRQHandler(void)
{
    tim_4_isr();
    TIM4->SR &= ~TIM_SR_UIF;
}

void TIM5_IRQHandler(void)
{
    tim_5_isr();
    TIM5->SR &= ~TIM_SR_UIF;
}

void TIM6_DAC_IRQHandler(void)
{
    tim_6_isr();
    TIM6->SR &= ~TIM_SR_UIF;
}

void TIM7_IRQHandler(void)
{
    tim_7_isr();
    TIM7->SR &= ~TIM_SR_UIF;
}

void TIM8_CC_IRQHandler(void)
{
    tim_8_isr();
    TIM8->SR &= ~TIM_SR_UIF;
}

void TIM1_BRK_TIM9_IRQHandler(void)
{
    tim_9_isr();
    TIM9->SR &= ~TIM_SR_UIF;
}

void TIM1_UP_TIM10_IRQHandler(void)
{
    tim_10_isr();
    TIM10->SR &= ~TIM_SR_UIF;
}

void TIM1_TRG_COM_TIM11_IRQHandler(void)
{
    tim_11_isr();
    TIM11->SR &= ~TIM_SR_UIF;
}

void TIM8_BRK_TIM12_IRQHandler(void)
{
    tim_12_isr();
    TIM12->SR &= ~TIM_SR_UIF;
}

void TIM8_UP_TIM13_IRQHandler(void)
{
    tim_13_isr();
    TIM13->SR &= ~TIM_SR_UIF;
}

void TIM8_TRG_COM_TIM14_IRQHandler(void)
{
    tim_14_isr();
    TIM14->SR &= ~TIM_SR_UIF;
}

bool hal_time_start_timer(uint_fast8_t device,
                          uint32_t clock,
                          uint_fast16_t reload_value,
                          TimerFkt function)
{
    TIM_TypeDef* timer = get_timer_register_for(device);
    if((NULL == timer) || (0 == clock) || (NULL == function))
    {
        return false;
    }
    switch(device)
    {
    case  1: tim_1_isr =  function; break;
    case  2: tim_2_isr =  function; break;
    case  3: tim_3_isr =  function; break;
    case  4: tim_4_isr =  function; break;
    case  5: tim_5_isr =  function; break;
    case  6: tim_6_isr =  function; break;
    case  7: tim_7_isr =  function; break;
    case  8: tim_8_isr =  function; break;
    case  9: tim_9_isr =  function; break;
    case 10: tim_10_isr = function; break;
    case 11: tim_11_isr = function; break;
    case 12: tim_12_isr = function; break;
    case 13: tim_13_isr = function; break;
    case 14: tim_14_isr = function; break;
    }
    enable_clock_for_timer(device);
    set_irq_priority(device);
    timer->PSC = (uint16_t)(0xffff & ((FREQUENCY_OF_CPU_CLK / clock) - 1));
    timer->ARR = reload_value;
    timer->CNT = 0; // start counting at 0
    timer->DIER = TIM_DIER_UIE; // Interrupt enable
    timer->CR1 =0x0005; // Timer enable + Interrupt on overflow
    return true;
}
void hal_time_stop_timer(uint_fast8_t device)
{
    TIM_TypeDef* timer =  get_timer_register_for(device);
    if(NULL == timer)
    {
        return;
    }
    timer->CR1 = 0;
    timer->DIER = 0;
    timer->CNT = 0;
    disable_clock_for_timer(device);
    switch(device)
    {
    case  1: tim_1_isr =  &error_isr_on_stopped_timer; break;
    case  2: tim_2_isr =  &error_isr_on_stopped_timer; break;
    case  3: tim_3_isr =  &error_isr_on_stopped_timer; break;
    case  4: tim_4_isr =  &error_isr_on_stopped_timer; break;
    case  5: tim_5_isr =  &error_isr_on_stopped_timer; break;
    case  6: tim_6_isr =  &error_isr_on_stopped_timer; break;
    case  7: tim_7_isr =  &error_isr_on_stopped_timer; break;
    case  8: tim_8_isr =  &error_isr_on_stopped_timer; break;
    case  9: tim_9_isr =  &error_isr_on_stopped_timer; break;
    case 10: tim_10_isr = &error_isr_on_stopped_timer; break;
    case 11: tim_11_isr = &error_isr_on_stopped_timer; break;
    case 12: tim_12_isr = &error_isr_on_stopped_timer; break;
    case 13: tim_13_isr = &error_isr_on_stopped_timer; break;
    case 14: tim_14_isr = &error_isr_on_stopped_timer; break;
    }
}

bool hal_time_set_timer_reload(uint_fast8_t device, uint16_t reload_value)
{
    TIM_TypeDef* timer = get_timer_register_for(device);
    if(NULL == timer)
    {
        return false;
    }
    timer->ARR = reload_value;
    return true;
}

