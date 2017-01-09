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
#include <stdio.h>
#include "board_cfg.h"
#include "hal_debug.h"


#define PWM_FREQUENCY 500000 // TODO

static TIM_TypeDef* get_timer_register_for(uint_fast8_t device);
static void enable_clock_for_timer(uint_fast8_t device);
static void disable_clock_for_timer(uint_fast8_t device);
static void error_isr_on_stopped_timer(void);
static uint32_t getClockFrequencyForTimer(uint_fast8_t device);

void TIM1_CC_IRQHandler(void) __attribute__ ((interrupt ("IRQ")));
void TIM2_IRQHandler(void) __attribute__ ((interrupt ("IRQ")));
void TIM3_IRQHandler(void) __attribute__ ((interrupt ("IRQ")));
void TIM4_IRQHandler(void) __attribute__ ((interrupt ("IRQ")));
void TIM5_IRQHandler(void) __attribute__ ((interrupt ("IRQ")));
void TIM6_DAC_IRQHandler(void) __attribute__ ((interrupt ("IRQ")));
void TIM7_IRQHandler(void) __attribute__ ((interrupt ("IRQ")));
void TIM8_CC_IRQHandler(void) __attribute__ ((interrupt ("IRQ")));
void TIM1_BRK_TIM9_IRQHandler(void) __attribute__ ((interrupt ("IRQ")));
void TIM1_UP_TIM10_IRQHandler(void) __attribute__ ((interrupt ("IRQ")));
void TIM1_TRG_COM_TIM11_IRQHandler(void) __attribute__ ((interrupt ("IRQ")));
void TIM8_BRK_TIM12_IRQHandler(void) __attribute__ ((interrupt ("IRQ")));
void TIM8_UP_TIM13_IRQHandler(void) __attribute__ ((interrupt ("IRQ")));
void TIM8_TRG_COM_TIM14_IRQHandler(void) __attribute__ ((interrupt ("IRQ")));


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
static bool initialized = false;

void hal_time_init(void)
{
    if(true == initialized)
    {
        // initialize only once !
        return;
    }
    initialized = true;

    tim_1_isr  =  &error_isr_on_stopped_timer;
    tim_2_isr  =  &error_isr_on_stopped_timer;
    tim_3_isr  =  &error_isr_on_stopped_timer;
    tim_4_isr  =  &error_isr_on_stopped_timer;
    tim_5_isr  =  &error_isr_on_stopped_timer;
    tim_6_isr  =  &error_isr_on_stopped_timer;
    tim_7_isr  =  &error_isr_on_stopped_timer;
    tim_8_isr  =  &error_isr_on_stopped_timer;
    tim_9_isr  =  &error_isr_on_stopped_timer;
    tim_10_isr =  &error_isr_on_stopped_timer;
    tim_11_isr =  &error_isr_on_stopped_timer;
    tim_12_isr =  &error_isr_on_stopped_timer;
    tim_13_isr =  &error_isr_on_stopped_timer;
    tim_14_isr =  &error_isr_on_stopped_timer;
}

static void error_isr_on_stopped_timer(void)
{
    hal_cpu_report_issue(1);
    debug_line("ERROR: ISR called on stopped Timer !");
}

void hal_time_ms_sleep(uint_fast32_t ms)
{
    uint32_t tickend = hal_cpu_get_ms_tick() + ms;
    uint32_t curtick = hal_cpu_get_ms_tick();
    while(curtick < tickend)
    {
        curtick = hal_cpu_get_ms_tick();
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

static uint32_t getClockFrequencyForTimer(uint_fast8_t device)
{
    switch(device)
    {
    case  1:return FREQUENCY_OF_APB2;
    case  2:return FREQUENCY_OF_APB1;
    case  3:return FREQUENCY_OF_APB1;
    case  4:return FREQUENCY_OF_APB1;
    case  5:return FREQUENCY_OF_APB1;
    case  6:return FREQUENCY_OF_APB1;
    case  7:return FREQUENCY_OF_APB1;
    case  8:return FREQUENCY_OF_APB2;
    case  9:return FREQUENCY_OF_APB2;
    case 10:return FREQUENCY_OF_APB2;
    case 11:return FREQUENCY_OF_APB2;
    case 12:return FREQUENCY_OF_APB1;
    case 13:return FREQUENCY_OF_APB1;
    case 14:return FREQUENCY_OF_APB1;
    default: return FREQUENCY_OF_CPU_CLK;
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
    hal_set_isr1_led(true);
    tim_1_isr();
    TIM1->SR &= ~TIM_SR_UIF;
    asm volatile("" : "+r" (initialized));
    hal_set_isr1_led(false);
}

void TIM2_IRQHandler(void)
{
    hal_set_isr1_led(true);
    tim_2_isr();
    TIM2->SR &= ~TIM_SR_UIF;
    asm volatile("" : "+r" (initialized));
    hal_set_isr1_led(false);
}

void TIM3_IRQHandler(void)
{
    hal_set_isr1_led(true);
    tim_3_isr();
    TIM3->SR &= ~TIM_SR_UIF;
    asm volatile("" : "+r" (initialized));
    hal_set_isr1_led(false);
}

void TIM4_IRQHandler(void)
{
    hal_set_isr1_led(true);
    tim_4_isr();
    TIM4->SR &= ~TIM_SR_UIF;
    asm volatile("" : "+r" (initialized));
    hal_set_isr1_led(false);
}

void TIM5_IRQHandler(void)
{
    hal_set_isr1_led(true);
    tim_5_isr();
    TIM5->SR &= ~TIM_SR_UIF;
    asm volatile("" : "+r" (initialized));
    hal_set_isr1_led(false);
}

void TIM6_DAC_IRQHandler(void)
{
    hal_set_isr1_led(true);
    tim_6_isr();
    TIM6->SR &= ~TIM_SR_UIF;
    asm volatile("" : "+r" (initialized));
    hal_set_isr1_led(false);
}

void TIM7_IRQHandler(void)
{
    hal_set_isr1_led(true);
    tim_7_isr();
    TIM7->SR &= ~TIM_SR_UIF;
    asm volatile("" : "+r" (initialized));
    hal_set_isr1_led(false);
}

void TIM8_CC_IRQHandler(void)
{
    hal_set_isr1_led(true);
    tim_8_isr();
    TIM8->SR &= ~TIM_SR_UIF;
    asm volatile("" : "+r" (initialized));
    hal_set_isr1_led(false);
}

void TIM1_BRK_TIM9_IRQHandler(void)
{
    hal_set_isr1_led(true);
    tim_9_isr();
    TIM9->SR &= ~TIM_SR_UIF;
    asm volatile("" : "+r" (initialized));
    hal_set_isr1_led(false);
}

void TIM1_UP_TIM10_IRQHandler(void)
{
    hal_set_isr1_led(true);
    tim_10_isr();
    TIM10->SR &= ~TIM_SR_UIF;
    asm volatile("" : "+r" (initialized));
    hal_set_isr1_led(false);
}

void TIM1_TRG_COM_TIM11_IRQHandler(void)
{
    hal_set_isr1_led(true);
    tim_11_isr();
    TIM11->SR &= ~TIM_SR_UIF;
    asm volatile("" : "+r" (initialized));
    hal_set_isr1_led(false);
}

void TIM8_BRK_TIM12_IRQHandler(void)
{
    hal_set_isr1_led(true);
    tim_12_isr();
    TIM12->SR &= ~TIM_SR_UIF;
    asm volatile("" : "+r" (initialized));
    hal_set_isr1_led(false);
}

void TIM8_UP_TIM13_IRQHandler(void)
{
    hal_set_isr1_led(true);
    tim_13_isr();
    TIM13->SR &= ~TIM_SR_UIF;
    asm volatile("" : "+r" (initialized));
    hal_set_isr1_led(false);
}

void TIM8_TRG_COM_TIM14_IRQHandler(void)
{
    hal_set_isr1_led(true);
    tim_14_isr();
    TIM14->SR &= ~TIM_SR_UIF;
    asm volatile("" : "+r" (initialized));
    hal_set_isr1_led(false);
}

// used by hal_pwm:

bool hal_time_enable_pwm_for(uint_fast8_t device)
{
    TIM_TypeDef* timer = get_timer_register_for(device);
    if((NULL == timer) || (0 == PWM_FREQUENCY))
    {
        hal_cpu_report_issue(2);
        return false;
    }
    enable_clock_for_timer(device);
    timer->EGR   = 0x0021;  // ???
    timer->CNT   = 0; // start counting at 0
    timer->PSC   = 1; // MAx frequency is best // (uint16_t)(0xffff & ((getClockFrequencyForTimer(device) / PWM_FREQUENCY) - 1));
    timer->ARR   = 0x00010001; // might be a 32bit counter
    timer->CCR1  = 0;
    timer->CCR2  = 0;
    timer->CCR3  = 0;
    timer->CCR4  = 0;
    timer->CCMR1 = 0x6060; // Output PWM Mode 1
    timer->CCMR2 = 0x6060; // Output PWM Mode 1
    timer->CCER  = 0x1111; // Output on, Negative off
    timer->BDTR  = 0xc000; // no lock, no delay, Output enabled
    // SMCR = 0; DIER = 0; SR = 0; RCR = 0; DCR = 0; DMAR = 0;
    timer->CR2   = 0x2a34;
    timer->CR1   = 0x0081; // Timer enable + ARR is buffered(0x80)
    return true;
}

bool hal_time_set_PWM_for(uint_fast8_t device, uint_fast8_t channel, uint16_t pwm_value)
{
    TIM_TypeDef* timer = get_timer_register_for(device);
    if(NULL == timer)
    {
        return false;
    }
    switch(channel)
    {
    case 1: timer->CCR1 = pwm_value; break;
    case 2: timer->CCR2 = pwm_value; break;
    case 3: timer->CCR3 = pwm_value; break;
    case 4: timer->CCR4 = pwm_value; break;
    }
    return true;
}

bool hal_time_stop_pwm_for(uint_fast8_t device, uint_fast8_t channel)
{
    TIM_TypeDef* timer = get_timer_register_for(device);
    if(NULL == timer)
    {
        return false;
    }
    switch(channel)
    {
    case 1: timer->CCR1 = 0; break;
    case 2: timer->CCR2 = 0; break;
    case 3: timer->CCR3 = 0; break;
    case 4: timer->CCR4 = 0; break;
    }
    return true;
}

// end of used by hal_pwm

// used by buzzer/ stepper:

bool hal_time_start_timer(uint_fast8_t device,
                          uint32_t clock,
                          uint_fast16_t reload_value,
                          TimerFkt function)
{
    TIM_TypeDef* timer = get_timer_register_for(device);
    debug_line("Start timer %d (%x)", device, function);
    if((NULL == timer) || (0 == clock))
    {
        return false;
    }
    enable_clock_for_timer(device);
    if(NULL != function)
    {
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
        timer->DIER = TIM_DIER_UIE; // Interrupt enable
        set_irq_priority(device);
    }
    timer->PSC   = (uint16_t)(0xffff & ((getClockFrequencyForTimer(device) / clock) - 1));
    timer->ARR   = reload_value;
    timer->CCR1  = reload_value;
    timer->CCR2  = 0;
    timer->CCR3  = 0;
    timer->CCR4  = 0;
    timer->CNT   = 0; // start counting at 0
    timer->CCMR1 = 0x0030;
    timer->CCMR2 = 0;
    timer->BDTR  = 0;
    timer->CCER  = 1;
    timer->EGR   = 3;
    timer->CR2   = 0;
    timer->CR1   = 0x0081; // Timer enable + Interrupt on overflow
    return true;
}

void hal_time_stop_timer(uint_fast8_t device)
{
    TIM_TypeDef* timer =  get_timer_register_for(device);
    if(NULL == timer)
    {
        return;
    }
    debug_line("stopping timer %d", device);
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

#ifdef DEBUG_ACTIVE

void hal_time_print_Configuration(int timerNumber)
{
    TIM_TypeDef* timer = get_timer_register_for(timerNumber);
    switch(timerNumber)
    {
    case 1:
        debug_line("Printing Configuration of Timer %d :", timerNumber);
        if(tim_1_isr ==  &error_isr_on_stopped_timer)
        {
            debug_line("ISR Function : None");
        }
        else
        {
            debug_line("ISR Function : at address 0x%08x", &(*tim_1_isr));
        }
        break;

    case 2:
        debug_line("Printing Configuration of Timer %d :", timerNumber);
        if(tim_2_isr ==  &error_isr_on_stopped_timer)
        {
            debug_line("ISR Function : None");
        }
        else
        {
            debug_line("ISR Function : at address 0x%08x", &(*tim_2_isr));
        }
        break;

    case 3:
        debug_line("Printing Configuration of Timer %d :", timerNumber);
        if(tim_3_isr ==  &error_isr_on_stopped_timer)
        {
            debug_line("ISR Function : None");
        }
        else
        {
            debug_line("ISR Function : at address 0x%08x", &(*tim_3_isr));
        }
        break;

    case 4:
        debug_line("Printing Configuration of Timer %d :", timerNumber);
        if(tim_4_isr ==  &error_isr_on_stopped_timer)
        {
            debug_line("ISR Function : None");
        }
        else
        {
            debug_line("ISR Function : at address 0x%08x", &(*tim_4_isr));
        }
        break;

    case 5:
        debug_line("Printing Configuration of Timer %d :", timerNumber);
        if(tim_5_isr ==  &error_isr_on_stopped_timer)
        {
            debug_line("ISR Function : None");
        }
        else
        {
            debug_line("ISR Function : at address 0x%08x", &(*tim_5_isr));
        }
        break;

    case 6:
        debug_line("Printing Configuration of Timer %d :", timerNumber);
        if(tim_6_isr ==  &error_isr_on_stopped_timer)
        {
            debug_line("ISR Function : None");
        }
        else
        {
            debug_line("ISR Function : at address 0x%08x", &(*tim_6_isr));
        }
        break;

    case 7:
        debug_line("Printing Configuration of Timer %d :", timerNumber);
        if(tim_7_isr ==  &error_isr_on_stopped_timer)
        {
            debug_line("ISR Function : None");
        }
        else
        {
            debug_line("ISR Function : at address 0x%08x", &(*tim_7_isr));
        }
        break;

    case 8:
        debug_line("Printing Configuration of Timer %d :", timerNumber);
        if(tim_8_isr ==  &error_isr_on_stopped_timer)
        {
            debug_line("ISR Function : None");
        }
        else
        {
            debug_line("ISR Function : at address 0x%08x", &(*tim_8_isr));
        }
        break;

    case 9:
        debug_line("Printing Configuration of Timer %d :", timerNumber);
        if(tim_9_isr ==  &error_isr_on_stopped_timer)
        {
            debug_line("ISR Function : None");
        }
        else
        {
            debug_line("ISR Function : at address 0x%08x", &(*tim_9_isr));
        }
        break;

    case 10:
        debug_line("Printing Configuration of Timer %d :", timerNumber);
        if(tim_10_isr ==  &error_isr_on_stopped_timer)
        {
            debug_line("ISR Function : None");
        }
        else
        {
            debug_line("ISR Function : at address 0x%08x", &(*tim_10_isr));
        }
        break;

    case 11:
        debug_line("Printing Configuration of Timer %d :", timerNumber);
        if(tim_11_isr ==  &error_isr_on_stopped_timer)
        {
            debug_line("ISR Function : None");
        }
        else
        {
            debug_line("ISR Function : at address 0x%08x", &(*tim_11_isr));
        }
        break;

    case 12:
        debug_line("Printing Configuration of Timer %d :", timerNumber);
        if(tim_12_isr ==  &error_isr_on_stopped_timer)
        {
            debug_line("ISR Function : None");
        }
        else
        {
            debug_line("ISR Function : at address 0x%08x", &(*tim_12_isr));
        }
        break;

    case 13:
        debug_line("Printing Configuration of Timer %d :", timerNumber);
        if(tim_13_isr ==  &error_isr_on_stopped_timer)
        {
            debug_line("ISR Function : None");
        }
        else
        {
            debug_line("ISR Function : at address 0x%08x", &(*tim_13_isr));
        }
        break;

    case 14:
        debug_line("Printing Configuration of Timer %d :", timerNumber);
        if(tim_14_isr ==  &error_isr_on_stopped_timer)
        {
            debug_line("ISR Function : None");
        }
        else
        {
            debug_line("ISR Function : at address 0x%08x", &(*tim_14_isr));
        }
        break;

    default:
        debug_line("Invalid Timer Number %d! allowed = 1..14", timerNumber);
        return;
    }

    debug_line("Timer->ARR     = 0x%08x", timer->ARR);
    debug_line("Timer->BDTR    = 0x%08x", timer->BDTR);
    debug_line("Timer->CCER    = 0x%08x", timer->CCER);
    debug_line("Timer->CCMR1   = 0x%08x", timer->CCMR1);
    debug_line("Timer->CCMR2   = 0x%08x", timer->CCMR2);
    debug_line("Timer->CCR1    = 0x%08x", timer->CCR1);
    debug_line("Timer->CCR2    = 0x%08x", timer->CCR2);
    debug_line("Timer->CCR3    = 0x%08x", timer->CCR3);
    debug_line("Timer->CCR4    = 0x%08x", timer->CCR4);
    debug_line("Timer->CNT     = 0x%08x", timer->CNT);
    debug_line("Timer->CR1     = 0x%08x", timer->CR1);
    debug_line("Timer->CR2     = 0x%08x", timer->CR2);
    debug_line("Timer->DCR     = 0x%08x", timer->DCR);
    debug_line("Timer->DIER    = 0x%08x", timer->DIER);
    debug_line("Timer->DMAR    = 0x%08x", timer->DMAR);
    debug_line("Timer->EGR     = 0x%08x", timer->EGR);
    debug_line("Timer->PSC     = 0x%08x", timer->PSC);
    debug_line("Timer->RCR     = 0x%08x", timer->RCR);
    debug_line("Timer->SMCR    = 0x%08x", timer->SMCR);
    debug_line("Timer->SR      = 0x%08x", timer->SR);
}

#endif // debug

// end of file
