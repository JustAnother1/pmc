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

#include <407discovery/board_cfg.h>
#include "hal_common.h"
#include "hal_cpu.h"
#include "hal_led.h"
#include "hal_time.h"
#include "flash.h"
#include "rcc.h"
#include <stddef.h>
#include <stdlib.h>

#define HEART_BEAT_FAST_LIMIT 5
#define HEART_BEAT_SLOW_LIMIT 500
#define HEART_BEAT_STEP_SIZE  5

struct tick_node {
    msTickFkt tick;
    struct tick_node * next;
};
typedef struct tick_node tick_entry;

static void hal_cpu_start_ms_timer(void);
static tick_entry * allocateNewEntry(void);

static tick_entry * tick_list = NULL;


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
  SCB->CPACR |= 0x00f00000;  /* set CP10 and CP11 Full Access */
  // Configure the Vector Table location add offset address
  SCB->VTOR = FLASH_BASE; // Vector Table Relocation in Internal FLASH
}

void hal_cpu_init_hal(void)
{
    // Power
    RCC->APB1ENR |= (RCC_APB1ENR_PWREN);
    MODIFY_REG(PWR->CR, PWR_CR_VOS, ((uint32_t)0x00008000));

    // FLASH
    // Enable Cache and prefetch for performance
    // configure wait states
    FLASH->ACR = (FLASH_ACR_LATENCY & WAIT_STATES)
               + FLASH_ACR_PRFTEN
               + FLASH_ACR_ICEN
               + FLASH_ACR_DCEN;
    // RCC - Reset and Clock Control
    RCC->PLLCFGR = (RCC->PLLCFGR & 0xf0bc8000) // preserve Reserved Bits
                 + RCC_PLL_PLLQ
                 + RCC_PLLCFGR_PLLSRC_HSE
                 + RCC_PLL_PLLP
                 + RCC_PLL_PLLN
                 + RCC_PLL_PLLM;
    RCC->CR = RCC_CR_PLLON
            + RCC_CR_HSEON
            + (RCC->CR & 0x0000fff8); // preserve calibration and trimming bits
    // Disable all interrupts
    RCC->CIR = 0x00000000;
    do
    {
        // wait for High Speed External (HSE) Clock to become ready.
        ;
    }while(0 == (RCC->CR & RCC_CR_HSERDY));
    do
    {
        // wait for High Speed External (HSE) Clock to become ready.
        ;
    }while(0 == (RCC->CR & RCC_CR_PLLRDY));

    RCC->CFGR = RCC_PRESC_APB2
              + RCC_PRESC_APB1
              + RCC_PRESC_AHB
              + RCC_SYS_CLK_SW;
    do
    {
        // wait for High Speed External (HSE) Clock to become ready.
        ;
    }while(0x00000008 != (RCC->CFGR & RCC_CFGR_SWS));
}

static tick_entry * allocateNewEntry(void)
{
    tick_entry * theMsTick = (struct tick_node *) malloc(sizeof(struct tick_node));
    if(NULL == theMsTick)
    {
        for(;;){;} // Time not yet working
    }
    theMsTick->tick = NULL;
    theMsTick->next = NULL;
    return theMsTick;
}

void hal_cpu_add_ms_tick_function(msTickFkt additional_function)
{
    if(NULL != additional_function)
    {
        if(NULL == tick_list)
        {
            tick_list = allocateNewEntry();
        }
        tick_entry *cur = tick_list;
        bool done = false;
        do
        {
            if(NULL == cur->tick)
            {
                // No tick in this Entry
                cur->tick = additional_function;
                done = true;
            }
            else
            {
                if(NULL == cur->next)
                {
                    // create a new entry
                    cur->next = allocateNewEntry();
                    cur = cur->next;
                }
                else
                {
                    // goto next entry
                    cur = cur->next;
                }
            }
        } while(false == done);
        hal_cpu_start_ms_timer();
    }
}

void SysTick_Handler(void)
{
    tick_entry *cur = tick_list;
    while(NULL != cur)
    {
        if(NULL != cur->tick)
        {
            // Execute that function
            (*cur->tick)();
        }
        cur = cur->next;
    }
}

// Configure SysTick to generate an interrupt every millisecond
static void hal_cpu_start_ms_timer(void)
{
    // set reload register
    SysTick->LOAD  = (FREQUENCY_OF_HCLK /1000) - 1;
    // set Priority for Systick Interrupt
    NVIC_SetPriority(SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);
    // make sure that counter starts at 0 now
    SysTick->VAL   = 0;
    // Enable SysTick IRQ and SysTick Timer ClkSrc = AHB
    SysTick->CTRL  = 7;
}

void hal_cpu_die(void)
{
    uint_fast32_t i = HEART_BEAT_FAST_LIMIT;
    bool direction_is_increment = true;
    for(;;)
    {
        /*
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
        */
    }
}

void hal_cpu_do_software_reset(void)
{
    NVIC_SystemReset();
}

// end of file
