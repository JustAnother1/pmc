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

#include <stdbool.h>
#include <stdio.h>
#include "st_gpio.h"
#include "st_rcc.h"
#include "st_util.h"
#include "st_exti.h"
#include "st_syscfg.h"
#include "hal_din.h"
#include "hal_led.h"
#include "board_cfg.h"
#include "hal_debug.h"

static bool initialized = false;

void EXTI0_IRQHandler(void) __attribute__ ((interrupt ("IRQ")));
void EXTI1_IRQHandler(void) __attribute__ ((interrupt ("IRQ")));
void EXTI2_IRQHandler(void) __attribute__ ((interrupt ("IRQ")));
void EXTI3_IRQHandler(void) __attribute__ ((interrupt ("IRQ")));
void EXTI4_IRQHandler(void) __attribute__ ((interrupt ("IRQ")));
void EXTI9_5_IRQHandler(void) __attribute__ ((interrupt ("IRQ")));
void EXTI15_10_IRQHandler(void) __attribute__ ((interrupt ("IRQ")));
static void check_pin(void);

static din_func funcs[D_IN_NUM_PINS];
static uint_fast8_t steppers[D_IN_NUM_PINS];
static uint_fast8_t last_state[D_IN_NUM_PINS];

void hal_din_init(void)
{
    int i;
    if(true == initialized)
    {
        // initialize only once !
        return;
    }
    initialized = true;

    for(i = 0; i < D_IN_NUM_PINS; i++)
    {
        funcs[i] = NULL;
        steppers[i] = 0xff;
        last_state[i] = 0;
    }

    // configure the pins

#if D_IN_NUM_PINS > 0
        RCC->AHB1ENR |= D_IN_0_RCC_GPIO_ENABLE;
        D_IN_0_GPIO_PORT->MODER   &= ~D_IN_0_MODER_0;
        D_IN_0_GPIO_PORT->MODER   |=  D_IN_0_MODER_1;
        D_IN_0_GPIO_PORT->OTYPER  &= ~D_IN_0_OTYPER_0;
        D_IN_0_GPIO_PORT->OTYPER  |=  D_IN_0_OTYPER_1;
        D_IN_0_GPIO_PORT->OSPEEDR &= ~D_IN_0_OSPEEDR_0;
        D_IN_0_GPIO_PORT->OSPEEDR |=  D_IN_0_OSPEEDR_1;
        D_IN_0_GPIO_PORT->PUPDR   &= ~D_IN_0_PUPD_0;
        D_IN_0_GPIO_PORT->PUPDR   |=  D_IN_0_PUPD_1;
#endif

#if D_IN_NUM_PINS > 1
        RCC->AHB1ENR |= D_IN_1_RCC_GPIO_ENABLE;
        D_IN_1_GPIO_PORT->MODER   &= ~D_IN_1_MODER_0;
        D_IN_1_GPIO_PORT->MODER   |=  D_IN_1_MODER_1;
        D_IN_1_GPIO_PORT->OTYPER  &= ~D_IN_1_OTYPER_0;
        D_IN_1_GPIO_PORT->OTYPER  |=  D_IN_1_OTYPER_1;
        D_IN_1_GPIO_PORT->OSPEEDR &= ~D_IN_1_OSPEEDR_0;
        D_IN_1_GPIO_PORT->OSPEEDR |=  D_IN_1_OSPEEDR_1;
        D_IN_1_GPIO_PORT->PUPDR   &= ~D_IN_1_PUPD_0;
        D_IN_1_GPIO_PORT->PUPDR   |=  D_IN_1_PUPD_1;
#endif

#if D_IN_NUM_PINS > 2
        RCC->AHB1ENR |= D_IN_2_RCC_GPIO_ENABLE;
        D_IN_2_GPIO_PORT->MODER   &= ~D_IN_2_MODER_0;
        D_IN_2_GPIO_PORT->MODER   |=  D_IN_2_MODER_1;
        D_IN_2_GPIO_PORT->OTYPER  &= ~D_IN_2_OTYPER_0;
        D_IN_2_GPIO_PORT->OTYPER  |=  D_IN_2_OTYPER_1;
        D_IN_2_GPIO_PORT->OSPEEDR &= ~D_IN_2_OSPEEDR_0;
        D_IN_2_GPIO_PORT->OSPEEDR |=  D_IN_2_OSPEEDR_1;
        D_IN_2_GPIO_PORT->PUPDR   &= ~D_IN_2_PUPD_0;
        D_IN_2_GPIO_PORT->PUPDR   |=  D_IN_2_PUPD_1;
#endif

#if D_IN_NUM_PINS > 3
        RCC->AHB1ENR |= D_IN_3_RCC_GPIO_ENABLE;
        D_IN_3_GPIO_PORT->MODER   &= ~D_IN_3_MODER_0;
        D_IN_3_GPIO_PORT->MODER   |=  D_IN_3_MODER_1;
        D_IN_3_GPIO_PORT->OTYPER  &= ~D_IN_3_OTYPER_0;
        D_IN_3_GPIO_PORT->OTYPER  |=  D_IN_3_OTYPER_1;
        D_IN_3_GPIO_PORT->OSPEEDR &= ~D_IN_3_OSPEEDR_0;
        D_IN_3_GPIO_PORT->OSPEEDR |=  D_IN_3_OSPEEDR_1;
        D_IN_3_GPIO_PORT->PUPDR   &= ~D_IN_3_PUPD_0;
        D_IN_3_GPIO_PORT->PUPDR   |=  D_IN_3_PUPD_1;
#endif

#if D_IN_NUM_PINS > 4
        RCC->AHB1ENR |= D_IN_4_RCC_GPIO_ENABLE;
        D_IN_4_GPIO_PORT->MODER   &= ~D_IN_4_MODER_0;
        D_IN_4_GPIO_PORT->MODER   |=  D_IN_4_MODER_1;
        D_IN_4_GPIO_PORT->OTYPER  &= ~D_IN_4_OTYPER_0;
        D_IN_4_GPIO_PORT->OTYPER  |=  D_IN_4_OTYPER_1;
        D_IN_4_GPIO_PORT->OSPEEDR &= ~D_IN_4_OSPEEDR_0;
        D_IN_4_GPIO_PORT->OSPEEDR |=  D_IN_4_OSPEEDR_1;
        D_IN_4_GPIO_PORT->PUPDR   &= ~D_IN_4_PUPD_0;
        D_IN_4_GPIO_PORT->PUPDR   |=  D_IN_4_PUPD_1;
#endif

#if D_IN_NUM_PINS > 5
        RCC->AHB1ENR |= D_IN_5_RCC_GPIO_ENABLE;
        D_IN_5_GPIO_PORT->MODER   &= ~D_IN_5_MODER_0;
        D_IN_5_GPIO_PORT->MODER   |=  D_IN_5_MODER_1;
        D_IN_5_GPIO_PORT->OTYPER  &= ~D_IN_5_OTYPER_0;
        D_IN_5_GPIO_PORT->OTYPER  |=  D_IN_5_OTYPER_1;
        D_IN_5_GPIO_PORT->OSPEEDR &= ~D_IN_5_OSPEEDR_0;
        D_IN_5_GPIO_PORT->OSPEEDR |=  D_IN_5_OSPEEDR_1;
        D_IN_5_GPIO_PORT->PUPDR   &= ~D_IN_5_PUPD_0;
        D_IN_5_GPIO_PORT->PUPDR   |=  D_IN_5_PUPD_1;
#endif

        // enable clock for GPIO Port
        RCC->AHB1ENR |= EXTI_GPIO_PORT_RCC;
        // enable clock for SYSCFG
        RCC->APB2ENR |= EXTI_APB2ENR;

        // Enable Interrupt
        NVIC_SetPriority(EXTI_0_IRQ_NUMBER, EXTI_0_IRQ_PRIORITY);
        NVIC_EnableIRQ(EXTI_0_IRQ_NUMBER);
        NVIC_SetPriority(EXTI_1_IRQ_NUMBER, EXTI_1_IRQ_PRIORITY);
        NVIC_EnableIRQ(EXTI_1_IRQ_NUMBER);
        NVIC_SetPriority(EXTI_2_IRQ_NUMBER, EXTI_2_IRQ_PRIORITY);
        NVIC_EnableIRQ(EXTI_2_IRQ_NUMBER);
        NVIC_SetPriority(EXTI_3_IRQ_NUMBER, EXTI_3_IRQ_PRIORITY);
        NVIC_EnableIRQ(EXTI_3_IRQ_NUMBER);
        NVIC_SetPriority(EXTI_4_IRQ_NUMBER, EXTI_4_IRQ_PRIORITY);
        NVIC_EnableIRQ(EXTI_4_IRQ_NUMBER);
        NVIC_SetPriority(EXTI_5_9_IRQ_NUMBER, EXTI_5_9_IRQ_PRIORITY);
        NVIC_EnableIRQ(EXTI_5_9_IRQ_NUMBER);
        NVIC_SetPriority(EXTI_10_15_IRQ_NUMBER, EXTI_10_15_IRQ_PRIORITY);
        NVIC_EnableIRQ(EXTI_10_15_IRQ_NUMBER);

        // Enable Interrupts for state change on Input lines
        EXTI->SWIER = D_IN_EXTI_SWIER;
        EXTI->FTSR = D_IN_EXTI_FTSR;
        EXTI->RTSR = D_IN_EXTI_RTSR;
        EXTI->EMR = D_IN_EXTI_EMR;
        EXTI->IMR = D_IN_EXTI_IMR;
        // Map Interrupt Lines to Pins in GPIO Ports
        SYSCFG->EXTICR[0] = D_IN_SYSCFG_EXTICR0;
        SYSCFG->EXTICR[1] = D_IN_SYSCFG_EXTICR1;
        SYSCFG->EXTICR[2] = D_IN_SYSCFG_EXTICR2;
        SYSCFG->EXTICR[3] = D_IN_SYSCFG_EXTICR3;
}

uint_fast8_t hal_din_get_amount(void)
{
    return D_IN_NUM_PINS;
}

uint_fast8_t hal_din_get_name(uint_fast8_t device, uint8_t *position)
{
    if(device < D_OUT_NUM_PINS)
    {
        switch(device)
        {
#if D_IN_NUM_PINS > 0
        case 0: return copy_string(D_IN_0_NAME, position);
#endif
#if D_IN_NUM_PINS > 1
        case 1: return copy_string(D_IN_1_NAME, position);
#endif
#if D_IN_NUM_PINS > 2
        case 2: return copy_string(D_IN_2_NAME, position);
#endif
#if D_IN_NUM_PINS > 3
        case 3: return copy_string(D_IN_3_NAME, position);
#endif
#if D_IN_NUM_PINS > 4
        case 4: return copy_string(D_IN_4_NAME, position);
#endif
#if D_IN_NUM_PINS > 5
        case 5: return copy_string(D_IN_5_NAME, position);
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

uint_fast8_t hal_din_get_switch_state(uint_fast8_t device)
{
    if(device < D_IN_NUM_PINS)
    {
        switch(device)
        {
#if D_IN_NUM_PINS > 0
        case 0: if(0 !=(D_IN_0_GPIO_PORT->IDR & D_IN_0_IDR)) {return 1;} else {return 0;} break;
#endif
#if D_IN_NUM_PINS > 1
        case 1: if(0 !=(D_IN_1_GPIO_PORT->IDR & D_IN_1_IDR)) {return 1;} else {return 0;} break;
#endif
#if D_IN_NUM_PINS > 2
        case 2: if(0 !=(D_IN_2_GPIO_PORT->IDR & D_IN_2_IDR)) {return 1;} else {return 0;} break;
#endif
#if D_IN_NUM_PINS > 3
        case 3: if(0 !=(D_IN_3_GPIO_PORT->IDR & D_IN_3_IDR)) {return 1;} else {return 0;} break;
#endif
#if D_IN_NUM_PINS > 4
        case 4: if(0 !=(D_IN_4_GPIO_PORT->IDR & D_IN_4_IDR)) {return 1;} else {return 0;} break;
#endif
#if D_IN_NUM_PINS > 5
        case 5: if(0 !=(D_IN_5_GPIO_PORT->IDR & D_IN_5_IDR)) {return 1;} else {return 0;} break;
#endif
        default:
            debug_line("din pin(%d) not available!", device);
            break;
        }
    }
    else
    {
        debug_line("din pin(%d) not available!", device);
    }
    return 0;
}

void hal_din_subscribe_to_events(uint_fast8_t switch_number, uint_fast8_t stepper_number, din_func handle_func)
{
    funcs[switch_number] = handle_func;
    steppers[switch_number] = stepper_number;
}

void EXTI0_IRQHandler(void)
{
    hal_set_isr1_led(true);
    check_pin();
    EXTI->PR = 1;
    hal_set_isr1_led(false);
}

void EXTI1_IRQHandler(void)
{
    hal_set_isr1_led(true);
    check_pin();
    EXTI->PR = 2;
    hal_set_isr1_led(false);
}

void EXTI2_IRQHandler(void)
{
    hal_set_isr1_led(true);
    check_pin();
    EXTI->PR = 4;
    hal_set_isr1_led(false);
}

void EXTI3_IRQHandler(void)
{
    hal_set_isr1_led(true);
    check_pin();
    EXTI->PR = 8;
    hal_set_isr1_led(false);
}

void EXTI4_IRQHandler(void)
{
    hal_set_isr1_led(true);
    check_pin();
    EXTI->PR = 16;
    hal_set_isr1_led(false);
}

void EXTI9_5_IRQHandler(void)
{
    hal_set_isr1_led(true);
    check_pin();
    EXTI->PR = 0x3e0; // 9-5
    hal_set_isr1_led(false);
}

void EXTI15_10_IRQHandler(void)
{
    hal_set_isr1_led(true);
    check_pin();
    EXTI->PR = 0xfc00; // 9-5
    hal_set_isr1_led(false);
}

static void check_pin(void)
{
    int i;
    int cur_state;
    debug_line("End stop triggered");
    for(i = 0; i < D_IN_NUM_PINS; i++)
    {
        cur_state = hal_din_get_switch_state(i);
        if(last_state[i] != cur_state)
        {
            // this pin had a change
            last_state[i] = cur_state;
            if(NULL != funcs[i])
            {
                debug_line("Notify step");
                if(1 == cur_state)
                {
                    funcs[i](true,steppers[i], i);
                }
                else
                {
                    funcs[i](false,steppers[i], i);
                }
            }
            else
            {
                // nobody cares about this input
                debug_line("Switch %d not registered", i);
            }
        }
        // else no change on this pin
    }
}

#ifdef DEBUG_ACTIVE
void hal_din_print_PinConfiguration(uint_fast8_t port, int idx)
{
    GPIO_TypeDef * PortRegisters;
    debug_line("Configuration of pin %c %d :", port, idx);
    switch(port)
    {
    case 'A':
    case 'a':
        PortRegisters = GPIOA;
        break;

    case 'B':
    case 'b':
        PortRegisters = GPIOB;
        break;

    case 'C':
    case 'c':
        PortRegisters = GPIOC;
        break;

    case 'D':
    case 'd':
        PortRegisters = GPIOD;
        break;

    case 'E':
    case 'e':
        PortRegisters = GPIOE;
        break;

    case 'F':
    case 'f':
        PortRegisters = GPIOF;
        break;

    case 'G':
    case 'g':
        PortRegisters = GPIOG;
        break;

    case 'H':
    case 'h':
        PortRegisters = GPIOH;
        break;

    case 'I':
    case 'i':
        PortRegisters = GPIOI;
        break;

    default:
        debug_line("Invalid PortRegisters !");
        return;
    }
    print_gpio_pin_configuration(PortRegisters, idx);
}
#endif

