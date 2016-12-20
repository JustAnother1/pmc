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
#include "hal_cfg.h"
#include "hal_cpu.h"
#include "hal_debug.h"
#include "hal_led.h"
#include "hal_time.h"
#include "407discovery/board_cfg.h"
#include "st_common.h"
#include "st_flash.h"
#include "st_rcc.h"
#include "st_rtc.h"
#include "st_util.h"


#define HEART_BEAT_FAST_LIMIT 5
#define HEART_BEAT_SLOW_LIMIT 500
#define HEART_BEAT_STEP_SIZE  5
#define MAX_TICK_FUNC         10


struct tick_node {
    msTickFkt tick;
    int cycle;
    int nextCall;
};
typedef struct tick_node tick_entry;

static volatile uint32_t now;

static void hal_cpu_start_ms_timer(void);

static tick_entry  tick_list[MAX_TICK_FUNC];


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
    int i;
    for(i=0; i < MAX_TICK_FUNC; i++)
    {
        tick_list[i].tick = NULL;
    }
    // start time
    now = 0;
    hal_cpu_start_ms_timer();
    // CC Memory
    RCC->AHB1ENR |= RCC_AHB1ENR_CCMDATARAMEN;
    // Power
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    // Sys Cfg
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    // Power Scale 1 (for 168MHz, Brown out Reset activated)
    PWR->CR = 0x000040f0;

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

void hal_cpu_add_ms_tick_function(msTickFkt additional_function)
{
    hal_cpu_add_ms_tick_function_cycle(additional_function, 1);
}

void hal_cpu_add_ms_tick_function_cycle(msTickFkt additional_function, int everyMs)
{
    if(NULL != additional_function)
    {
        int i;
        for(i = 0; i < MAX_TICK_FUNC; i++)
        {
            if(NULL == tick_list[i].tick)
            {
                // No tick in this Entry
                tick_list[i].tick = additional_function;
                tick_list[i].cycle = everyMs;
                tick_list[i].nextCall = 1;
                return;
            }
            // else check next slot
        }
        // no more free slots
        debug_line("ERROR: Could not add ms Tick Function!");
        hal_cpu_report_issue(7);
        hal_set_error_led(true);
    }
}

uint32_t hal_cpu_get_ms_tick(void)
{
    return now;
}

void SysTick_Handler(void)
{
    now++;
}

// Configure SysTick to generate an interrupt every millisecond
static void hal_cpu_start_ms_timer(void)
{
    // set reload register
    SysTick->LOAD  = (FREQUENCY_OF_HCLK /1000) - 1;
    // set Priority for Systick Interrupt
    NVIC_SetPriority(SysTick_IRQn, 15); // set to lowest possible priority = 15
    // make sure that counter starts at 0 now
    SysTick->VAL   = 0;
    // Enable SysTick IRQ and SysTick Timer ClkSrc = AHB
    SysTick->CTRL  = 7;
}

void hal_cpu_die(void)
{
    // TODO FIX
    uint_fast32_t i = HEART_BEAT_FAST_LIMIT;
    bool direction_is_increment = true;
    // Disable all System Tick( = 1ms) services
    hal_cpu_report_issue(8);
    for(i=0; i < MAX_TICK_FUNC; i++)
    {
        tick_list[i].tick = NULL;
    }
    hal_set_error_led(direction_is_increment);
    for(;;)
    {
        if((i <HEART_BEAT_FAST_LIMIT) && (false == direction_is_increment))
        {
            direction_is_increment = true;
            hal_set_error_led(direction_is_increment);
            i = HEART_BEAT_FAST_LIMIT;
        }
        else if((i > HEART_BEAT_SLOW_LIMIT) && (true == direction_is_increment))
        {
            direction_is_increment = false;
            hal_set_error_led(direction_is_increment);
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
        mDelay(i);
    }
}

void hal_cpu_do_software_reset(uint32_t reason)
{
    RTC->BKP0R = reason;
    NVIC_SystemReset();
}

static uint32_t lastTickAt = 0;

void hal_cpu_tick(void)
{
    // this gets called from the main loop.
    // the main loop should spin multiple times each millisecond.
    uint32_t curTick = hal_cpu_get_ms_tick();
    if(curTick != lastTickAt)
    {
        int i;
        for(i = 0; i < MAX_TICK_FUNC; i++)
        {
            tick_list[i].nextCall--;
            if(1 > tick_list[i].nextCall)
            {
                if(NULL != tick_list[i].tick)
                {
                    // Execute that function
                    (*tick_list[i].tick)();
                }
                tick_list[i].nextCall = tick_list[i].cycle;
            }
        }
        // for safety here:
        // If the tick functions take more than ms then we rather skip a ms than
        // to never stop ticking anymore.
        lastTickAt = curTick;
    }
}

// my own Fault Handlers
#ifdef DEBUG_ACTIVE
void hal_cpu_check_Reset_Reason(void)
{
    // Reset Reason in RCC:
    // ====================
    debug_msg("Reset Reason: ");
    uint32_t resetSource = RCC->CSR;
    if(0 != (resetSource & RCC_CSR_LPWRRSTF))
    {
        debug_line("Low Power");
    }
    if(0 != (resetSource & RCC_CSR_WWDGRSTF))
    {
        debug_line("Window Watchdog");
    }
    if(0 != (resetSource & RCC_CSR_WDGRSTF))
    {
        debug_line("Independent Watchdog");
    }
    if(0 != (resetSource & RCC_CSR_SFTRSTF))
    {
        debug_line("Software");
    }
    if(0 != (resetSource & RCC_CSR_PORRSTF))
    {
        debug_line("POR/PDR");
    }
    if(0 != (resetSource & RCC_CSR_PADRSTF))
    {
        debug_line("Reset Pin Signal");
    }
    if(0 != (resetSource & RCC_CSR_BORRSTF))
    {
        debug_line("Brown out Reset");
    }
    // reset Flags
    resetSource    |= RCC_CSR_RMVF;
    RCC->CSR = resetSource;

    // Reset Reason reported by this software :
    // ========================================

    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR |= PWR_CR_DBP;

    switch(RTC->BKP0R)
    {

    case RESET_REASON_NO_REASON:
        debug_line("Reason Detail: No Reason reported");
        break;

    case RESET_REASON_HOST_ORDER:
        debug_line("Reason Detail: Reset ordered by Host");
        break;

    case RESET_REASON_DEBUG_USER_REQUEST:
        debug_line("Reason Detail: Reset ordered by Debug User");
        break;

    case RESET_REASON_HAL:
        debug_line("Reason Detail: HAL with no Reason reported");
        break;

    case RESET_REASON_HAL | 1:
        debug_line("Reason Detail: Non Maskable Interrupt");
        break;

    case RESET_REASON_HAL | 2:
        debug_line("Reason Detail: Hard Fault");
        break;

    case RESET_REASON_HAL | 3:
        debug_line("Reason Detail: Memory Management");
        break;

    case RESET_REASON_HAL | 4:
        debug_line("Reason Detail: Bus Fault");
        break;

    case RESET_REASON_HAL | 5:
        debug_line("Reason Detail: Usage Fault");
        break;

    case RESET_REASON_HAL | 6:
        debug_line("Reason Detail: System Service call via SWI instruction (SVC)");
        break;

    case RESET_REASON_HAL | 7:
        debug_line("Reason Detail: Debug Monitor");
        break;

    case RESET_REASON_HAL | 8:
        debug_line("Reason Detail: Pendable request for system service (Pend SV)");
        break;

    case RESET_REASON_HAL | 9:
        debug_line("Reason Detail: Floating Point Unit Interrupt");
        break;

    default:
        debug_line("Reason Detail: 0x%08X", RTC->BKP0R);
        break;
    }
    RTC->BKP0R = RESET_REASON_NO_REASON;

    // Issue reported by this software :
    // =================================
    // Issue numbers 1-10 !
    if(0 != RTC->BKP1R)
    {
        debug_line("Reported issue 1: %d", RTC->BKP1R);
        RTC->BKP1R = 0;
    }
    if(0 != RTC->BKP2R)
    {
        debug_line("Reported issue 2: %d", RTC->BKP2R);
        RTC->BKP2R = 0;
    }
    if(0 != RTC->BKP3R)
    {
        debug_line("Reported issue 3: %d", RTC->BKP3R);
        RTC->BKP3R = 0;
    }
    if(0 != RTC->BKP4R)
    {
        debug_line("Reported issue 4: %d", RTC->BKP4R);
        RTC->BKP4R = 0;
    }
    if(0 != RTC->BKP5R)
    {
        debug_line("Reported issue 5: %d", RTC->BKP5R);
        RTC->BKP5R = 0;
    }
    if(0 != RTC->BKP6R)
    {
        debug_line("Reported issue 6: %d", RTC->BKP6R);
        RTC->BKP6R = 0;
    }
    if(0 != RTC->BKP7R)
    {
        debug_line("Reported issue 7: %d", RTC->BKP7R);
        RTC->BKP7R = 0;
    }
    if(0 != RTC->BKP8R)
    {
        debug_line("Reported issue 8: %d", RTC->BKP8R);
        RTC->BKP8R = 0;
    }
    if(0 != RTC->BKP9R)
    {
        debug_line("Reported issue 9: %d", RTC->BKP9R);
        RTC->BKP9R = 0;
    }
    if(0 != RTC->BKP10R)
    {
        debug_line("Reported issue 10: %d", RTC->BKP10R);
        RTC->BKP10R = 0;
    }
}
#endif

void hal_cpu_report_issue(uint32_t issue_number)
{
    if(0 == RTC->BKP1R)
    {
        RTC->BKP1R = issue_number;
    }
    else if(0 == RTC->BKP2R)
    {
        RTC->BKP2R = issue_number;
    }
    else if(0 == RTC->BKP3R)
    {
        RTC->BKP3R = issue_number;
    }
    else if(0 == RTC->BKP4R)
    {
        RTC->BKP4R = issue_number;
    }
    else if(0 == RTC->BKP5R)
    {
        RTC->BKP5R = issue_number;
    }
    else if(0 == RTC->BKP6R)
    {
        RTC->BKP6R = issue_number;
    }
    else if(0 == RTC->BKP7R)
    {
        RTC->BKP7R = issue_number;
    }
    else if(0 == RTC->BKP8R)
    {
        RTC->BKP8R = issue_number;
    }
    else if(0 == RTC->BKP9R)
    {
        RTC->BKP9R = issue_number;
    }
    else if(0 == RTC->BKP10R)
    {
        RTC->BKP10R = issue_number;
    }
    else
    {
        RTC->BKP10R = 0xffffffff;
    }
    // TODO allow for more than one issue
}

#ifdef DEBUG_ACTIVE
void hal_cpu_print_Interrupt_information(void)
{
    int i = 0;
    int numTasks = 0;
    // NVIC

    debug_line("NVIC :");
    debug_line("NVIC->ISER[0] :%08X", NVIC->ISER[0]);
    debug_line("NVIC->ISER[1] :%08X", NVIC->ISER[1]);
    debug_line("NVIC->ISER[2] :%08X", NVIC->ISER[2]);

    debug_line("NVIC->ICER[0] :%08X", NVIC->ICER[0]);
    debug_line("NVIC->ICER[1] :%08X", NVIC->ICER[1]);
    debug_line("NVIC->ICER[2] :%08X", NVIC->ICER[2]);

    debug_line("NVIC->ISPR[0] :%08X", NVIC->ISPR[0]);
    debug_line("NVIC->ISPR[1] :%08X", NVIC->ISPR[1]);
    debug_line("NVIC->ISPR[2] :%08X", NVIC->ISPR[2]);

    debug_line("NVIC->ICPR[0] :%08X", NVIC->ICPR[0]);
    debug_line("NVIC->ICPR[1] :%08X", NVIC->ICPR[1]);
    debug_line("NVIC->ICPR[2] :%08X", NVIC->ICPR[2]);


    debug_line("NVIC->IABR[0] :%08X", NVIC->IABR[0]);
    debug_line("NVIC->IABR[1] :%08X", NVIC->IABR[1]);
    debug_line("NVIC->IABR[2] :%08X", NVIC->IABR[2]);

    for(i = 0; i < 240; i++)
    {
        if(0 != NVIC->IP[i])
        {
            debug_line("NVIC->IP[%2d]  :%02X", i, NVIC->IP[i]);
        }
    }

    // My "Tasks"
    i = 0;
    debug_line("Tasks :");
    for(i = 0; i < MAX_TICK_FUNC; i++)
    {
        if(NULL != tick_list[i].tick)
        {
            debug_line("Task with Cycle Time %3d and function at 0x%X", tick_list[i].cycle, &(tick_list[i].tick));
            numTasks++;
        }
        // else empty slot
    }
    debug_line("%d tasks.", i);
}
#endif

void NMI_Handler(void)
{
    RTC->BKP0R = RESET_REASON_HAL | 1;
    NVIC_SystemReset();
}

void HardFault_Handler(void)
{
    RTC->BKP0R = RESET_REASON_HAL | 2;
    NVIC_SystemReset();
}

void MemManage_Handler(void)
{
    RTC->BKP0R = RESET_REASON_HAL | 3;
    NVIC_SystemReset();
}

void BusFault_Handler(void)
{
    RTC->BKP0R = RESET_REASON_HAL | 4;
    NVIC_SystemReset();
}

void UsageFault_Handler(void)
{
    RTC->BKP0R = RESET_REASON_HAL | 5;
    NVIC_SystemReset();
}

void SVC_Handler(void)
{
    RTC->BKP0R = RESET_REASON_HAL | 6;
    NVIC_SystemReset();
}

void DebugMon_Handler(void)
{
    RTC->BKP0R = RESET_REASON_HAL | 7;
    NVIC_SystemReset();
}

void PendSV_Handler(void)
{
    RTC->BKP0R = RESET_REASON_HAL | 8;
    NVIC_SystemReset();
}

void FPU_IRQHandler(void)
{
    RTC->BKP0R = RESET_REASON_HAL | 9;
    NVIC_SystemReset();
}

// end of file
