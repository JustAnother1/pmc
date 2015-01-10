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

#include "hal_common.h"
#include "hal_cpu.h"
#include "hal_led.h"
#include "hal_time.h"
#include "cpu_cfg.h"
#include "flash.h"
#include "rcc.h"
#include <stddef.h>

#define HEART_BEAT_FAST_LIMIT 5
#define HEART_BEAT_SLOW_LIMIT 500
#define HEART_BEAT_STEP_SIZE  5

static void hal_cpu_start_ms_timer(void);

static msTickFkt theMsTick = NULL;
static bool ms_tick_active = false;


/**
  * @brief  Setup the microcontroller system
  *         Initialize the FPU setting, vector table location and External memory
  *         configuration.
  *         gets called from startup.s before main()
  * @param  None
  * @retval None
  */
void SystemInit(void)
{
  // FPU settings
  SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  /* set CP10 and CP11 Full Access */
  // Configure the Vector Table location add offset address
  SCB->VTOR = FLASH_BASE; // Vector Table Relocation in Internal FLASH
}


void hal_cpu_init_hal(void)
{
    // FLASH
    // Enable Cache and prefetch for performance
    // configure wait states
    FLASH->ACR = (FLASH_ACR_LATENCY & WAIT_STATES)
               + FLASH_ACR_PRFTEN
               + FLASH_ACR_ICEN
               + FLASH_ACR_DCEN;
    // RCC - Reset and Clock Control
    RCC->CR = RCC_CR_PLLON
            + RCC_CR_HSEON
            + (RCC->CR & 0xfffe); // preserve calibration and trimming bits
    RCC->PLLCFGR = (RCC->PLLCFGR & 0xf0bc8000) // preserve Reserved Bits
                 + RCC_PLL_PLLQ
                 + RCC_PLLCFGR_PLLSRC_HSE
                 + RCC_PLL_PLLP
                 + RCC_PLL_PLLN
                 + RCC_PLL_PLLM;
    RCC->CFGR = RCC_MC02
              + RCC_MC01
              + RCC_PRESC_APB2
              + RCC_PRESC_APB1
              + RCC_PRESC_AHB
              + RCC_SYS_CLK_SW;
    // Disable all interrupts
    RCC->CIR = 0x00000000;
    RCC->AHB1ENR = (RCC->AHB1ENR & 0x8183EE00)
                 + RCC_AHB1ENR_DMA2EN
                 + RCC_AHB1ENR_DMA1EN
                 + RCC_AHB1ENR_CCMDATARAMEN
                 + RCC_AHB1ENR_GPIODEN;
    // TODO other GPIO
    RCC->APB1ENR = (RCC->APB1ENR & 0xc9013600)
                 + RCC_APB1ENR_PWREN
                 + RCC_APB1ENR_USART2EN;
    // TODO Timer enable, I2C SPI,..
    RCC->APB2ENR = (RCC->APB2ENR & 0xfff8a0cc)
                 + RCC_APB2ENR_SPI1EN
                 + RCC_APB2ENR_USART1EN;
    // TODO ADC + TIM
}

void hal_cpu_complete_init(void)
{
    if(true == ms_tick_active)
    {
        hal_cpu_start_ms_timer();
    }
}

void hal_cpu_add_ms_tick_function(msTickFkt additional_function)
{
    if(NULL != additional_function)
    {
        theMsTick = additional_function;
        ms_tick_active = true;
    }
}

void SysTick_Handler(void)
{
    if(NULL != theMsTick)
    {
        (*theMsTick)();
    }
}

// Configure SysTick to generate an interrupt every millisecond
static void hal_cpu_start_ms_timer(void)
{
    if (((FREQUENCY_OF_SYSTEM_CORE_CLOCK /1000) - 1) > SysTick_LOAD_RELOAD_Msk)
    {
        // Reload value impossible
        hal_cpu_die();
    }
    // else :
    SysTick->LOAD  = (FREQUENCY_OF_SYSTEM_CORE_CLOCK /1000) - 1; // set reload register
    NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);  // set Priority for Systick Interrupt
    SysTick->VAL   = 0;                                          // Load the SysTick Counter Value
    SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                     SysTick_CTRL_TICKINT_Msk   |
                     SysTick_CTRL_ENABLE_Msk;                    // Enable SysTick IRQ and SysTick Timer
}

void hal_cpu_die(void)
{
    uint_fast32_t i = HEART_BEAT_FAST_LIMIT;
    bool direction_is_increment = true;
    for(;;)
    {
        if((i <HEART_BEAT_FAST_LIMIT) && (false == direction_is_increment))
        {
            direction_is_increment = true;
            i = HEART_BEAT_FAST_LIMIT;
        }
        else if((i > HEART_BEAT_SLOW_LIMIT) && (true == direction_is_increment))
        {
            direction_is_increment = false;
            i = HEART_BEAT_SLOW_LIMIT;
        }
        else if(true == direction_is_increment)
        {
            i = i + HEART_BEAT_STEP_SIZE;
        }
        else
        {
            i = i - HEART_BEAT_STEP_SIZE;
        }
        hal_led_toggle_debug_led();
        hal_time_ms_sleep(i);
    }
}

void hal_cpu_do_software_reset(void)
{
    NVIC_SystemReset();
}

// end of file
