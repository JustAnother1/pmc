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
#include <stdbool.h>
#include "st_adc.h"
#include "st_gpio.h"
#include "st_dma.h"
#include "hal_adc.h"
#include "board_cfg.h"
#include "hal_debug.h"
#include "st_rcc.h"
#include "st_util.h"

#define ADC_USE_DMA 0

#define NUM_TEMPERATURES   ADC_NUM_PINS + 1

#if 1 == ADC_USE_DMA
static uint16_t res_buf[NUM_TEMPERATURES];
#endif

void hal_adc_init(void)
{
#if 1 == ADC_USE_DMA
	int i;
	for(i = 0; i < NUM_TEMPERATURES; i++)
	{
		res_buf[i] = 7; // -> 0.7 °C ;-)
	}
	// Power ON DMA2
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
#endif
	// Power on ADC
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    // enable clock for GPIO Port
    RCC->AHB1ENR |= ADC_0_GPIO_PORT_RCC;
    RCC->AHB1ENR |= ADC_1_GPIO_PORT_RCC;
    RCC->AHB1ENR |= ADC_2_GPIO_PORT_RCC;
    RCC->AHB1ENR |= ADC_3_GPIO_PORT_RCC;

#if 1 == ADC_USE_DMA
	DMA2_Stream0->PAR = ADC1->DR;  // Peripheral Address
	// DMA2_Stream0->PAR = ADC1;  // Peripheral Address
	DMA2_Stream0->M0AR = (uint32_t)&(res_buf[0]);  // Memory Address
	DMA2_Stream0->NDTR = ADC_NUM_PINS + 1; //number of transfers before resetting the pointer
	DMA2_Stream0->FCR = 0; // Direct mode
	// Stream 0, Channel 0
	// single transfer, no double buffer, priority low, peripheral to memory, DMA is flow controller
	DMA2_Stream0->CR = (1 <<13) // 16bit transfer
			         | (1 <<11) // 16bit transfer
					 | (1 <<10) // increment pointer, peripheral pointer is fixed
					 | (1 <<8)  // circular mode
					 | (1 <<1)  // Direct more Error Interrupt enabled
					 | (1 <<0); // Stream enable
#endif

	ADC_0_GPIO_PORT->MODER   |=  ADC_0_GPIO_MODER_1;
	ADC_0_GPIO_PORT->MODER   &= ~ADC_0_GPIO_MODER_0;
	ADC_1_GPIO_PORT->MODER   |=  ADC_1_GPIO_MODER_1;
	ADC_1_GPIO_PORT->MODER   &= ~ADC_1_GPIO_MODER_0;
	ADC_2_GPIO_PORT->MODER   |=  ADC_2_GPIO_MODER_1;
	ADC_2_GPIO_PORT->MODER   &= ~ADC_2_GPIO_MODER_0;
	ADC_3_GPIO_PORT->MODER   |=  ADC_3_GPIO_MODER_1;
	ADC_3_GPIO_PORT->MODER   &= ~ADC_3_GPIO_MODER_0;
	// TODO more than 4 Temperature sensor Input Pins

#if 1 == ADC_USE_DMA
	// set list of channels that shall be measured
	ADC1->SQR3 = ((0x1f & ADC_0_INPUT_NUM)   << 0)
			   | ((0x1f & ADC_1_INPUT_NUM)   << 5)
			   | ((0x1f & ADC_2_INPUT_NUM)   << 10)
			   | ((0x1f & ADC_3_INPUT_NUM)   << 15)
			   | ((0x1f & ADC_4_INPUT_NUM)   << 20)
			   | ((0x1f & ADC_5_INPUT_NUM)   << 25);
	ADC1->SQR2 = ((0x1f & ADC_6_INPUT_NUM)   << 0)
			   | ((0x1f & ADC_7_INPUT_NUM)   << 5)
			   | ((0x1f & ADC_8_INPUT_NUM)   << 10)
			   | ((0x1f & ADC_9_INPUT_NUM)   << 15)
			   | ((0x1f & ADC_10_INPUT_NUM)  << 20)
			   | ((0x1f & ADC_11_INPUT_NUM)  << 25);
	ADC1->SQR1 = ((0x1f & ADC_12_INPUT_NUM)  << 0)
			   | ((0x1f & ADC_13_INPUT_NUM)  << 5)
			   | ((0x1f & ADC_14_INPUT_NUM)  << 10)
			   | ((0x1f & ADC_15_INPUT_NUM)  << 15)
			   // Number of conversions = ADC_NUM_PINS + internal Temperature Sensor
			   | ((0xf & (ADC_NUM_PINS + 1)) << 20);
#endif
	ADC1->SR = 0; // Clear interrupt flags
	// Enable chip internal Temperature sensor
	// Clock prescaler - /4
	ADC->CCR = ADC_CCR_TSVREFE | ADC_CCR_ADCPRE_4;


	// sample really slowly - just in case
	ADC1->SMPR1 = 0x07ffffff;
	ADC1->SMPR2 = 0x3fffffff;

#if 1 == ADC_USE_DMA
	// 12bit: RES = 0
	// Scan mode,
	ADC1->CR1 = ADC_CR1_SCAN;
	// start conversion
	// right alignment (0 .. 4095)-> ALIGN = 0
	// DMA at every conversion EOCS = 1
	// DMA on -> DDS = 1, DMA = 1
	// Continuous conversion
	// ADC Power ON
	ADC1->CR2 = ADC_CR2_SWSTART | ADC_CR2_EOCS | ADC_CR2_DDS | ADC_CR2_DMA | ADC_CR2_CONT | ADC_CR2_ADON;
	// TODO? sample time
#else
	ADC1->CR1 = 0;
#endif
}

void hal_print_configuration_adc(void)
{
    debug_line("Configuration of ADC :");
    // Clock
    debug_line("RCC->AHB1ENR       = 0x%08x", RCC->AHB1ENR);
    debug_line("RCC->APB2ENR       = 0x%08x", RCC->APB2ENR);
#if 1 == ADC_USE_DMA
    // DMA
    debug_line("DMA 2 :");
    debug_line("DMA2->LISR         = 0x%08x", DMA2->LISR);
    debug_line("DMA2->HISR         = 0x%08x", DMA2->HISR);
    debug_line("DMA2->LIFCR        = 0x%08x", DMA2->LIFCR);
    debug_line("DMA2->HIFCR        = 0x%08x", DMA2->HIFCR);
    // DMA Stream
    debug_line("DMA 2 Stream 0 :");
    debug_line("DMA2_Stream0->CR   = 0x%08x", DMA2_Stream0->CR);
    debug_line("DMA2_Stream0->FCR  = 0x%08x", DMA2_Stream0->FCR);
    debug_line("DMA2_Stream0->M0AR = 0x%08x", DMA2_Stream0->M0AR);
    debug_line("DMA2_Stream0->M1AR = 0x%08x", DMA2_Stream0->M1AR);
    debug_line("DMA2_Stream0->NDTR = 0x%08x", DMA2_Stream0->NDTR);
    debug_line("DMA2_Stream0->PAR  = 0x%08x", DMA2_Stream0->PAR);
#endif
    // ADC
    debug_line("ADC1->SR           = 0x%08x", ADC1->SR);
    debug_line("ADC1->DR           = 0x%08x", ADC1->DR);
    debug_line("ADC1->CR1          = 0x%08x", ADC1->CR1);
    debug_line("ADC1->CR2          = 0x%08x", ADC1->CR2);
    debug_line("ADC1->SMPR1        = 0x%08x", ADC1->SMPR1);
    debug_line("ADC1->SMPR2        = 0x%08x", ADC1->SMPR2);
    debug_line("ADC1->JOFR1        = 0x%08x", ADC1->JOFR1);
    debug_line("ADC1->JOFR2        = 0x%08x", ADC1->JOFR2);
    debug_line("ADC1->JOFR3        = 0x%08x", ADC1->JOFR3);
    debug_line("ADC1->JOFR4        = 0x%08x", ADC1->JOFR4);
    debug_line("ADC1->HTR          = 0x%08x", ADC1->HTR);
    debug_line("ADC1->LTR          = 0x%08x", ADC1->LTR);
    debug_line("ADC1->SQR1         = 0x%08x", ADC1->SQR1);
    debug_line("ADC1->SQR2         = 0x%08x", ADC1->SQR2);
    debug_line("ADC1->SQR3         = 0x%08x", ADC1->SQR3);
    debug_line("ADC1->JSQR         = 0x%08x", ADC1->JSQR);
    debug_line("ADC1->JDR1         = 0x%08x", ADC1->JDR1);
    debug_line("ADC1->JDR2         = 0x%08x", ADC1->JDR2);
    debug_line("ADC1->JDR3         = 0x%08x", ADC1->JDR3);
    debug_line("ADC1->JDR4         = 0x%08x", ADC1->JDR4);

    // Pins
    print_gpio_configuration(GPIOA);
    print_gpio_configuration(GPIOC);
}

uint_fast8_t hal_adc_get_amount(void)
{
    return NUM_TEMPERATURES;
}

#define MAX_TRIES 500

uint_fast16_t hal_adc_get_value(uint_fast8_t device)
{
	if(device < NUM_TEMPERATURES)
	{
#if 1 == ADC_USE_DMA
		return res_buf[device];
#else
		int i = 0;
		switch(device)
		{
		case 0: ADC1->SQR3 = ADC_0_INPUT_NUM; break;
		case 1: ADC1->SQR3 = ADC_1_INPUT_NUM; break;
		case 2: ADC1->SQR3 = ADC_2_INPUT_NUM; break;
		case 3: ADC1->SQR3 = ADC_3_INPUT_NUM; break;
		default:
		case 4: ADC1->SQR3 = ADC_4_INPUT_NUM; break;
		}
		ADC1->CR2 = ADC_CR2_SWSTART | ADC_CR2_EOCS | ADC_CR2_ADON;
		while((0 == (ADC1->SR & 0x2)) && (i < MAX_TRIES))
		{
			i++;
		}
		if(MAX_TRIES == i)
		{
			debug_line("ADC never finished! Result %d !", ADC1->DR);
			return 7;
		}
		if(4 == device) // Temperature sensor
		{
			uint32_t help = ADC1->DR * 1000;
			help = help - 952087;
			help = help /341;
			return 0xffff & help;
		}
		else
		{
			return ADC1->DR;
		}
#endif
	}
	else
	{
		// TODO SPI Temperature Sensor?
		// invalid device number
		return 0;
	}
}

uint_fast8_t hal_adc_get_name(uint_fast8_t device, uint8_t *position)
{
    if(device < NUM_TEMPERATURES)
    {
        switch(device)
        {
        case  0: return copy_string(ADC_0_NAME, position);
        case  1: return copy_string(ADC_1_NAME, position);
        case  2: return copy_string(ADC_2_NAME, position);
        case  3: return copy_string(ADC_3_NAME, position);
        case  4: return copy_string(ADC_4_NAME, position);
        case  5: return copy_string(ADC_5_NAME, position);
        default:
            return 0;
        }
    }
    else
    {
        return 0;
    }
}

void DMA2_Stream0_IRQHandler(void)
{
	debug_line("ERROR: Unexpected DMA2:Stream0 Interrupt (Low: 0x%08x, High: 0x%08x) !",
			    DMA2->LISR, DMA2->HISR);
}

void ADC_IRQHandler(void)
{
	if(ADC_SR_OVR == (ADC1->SR & ADC_SR_OVR))
	{
		 debug_line("ERROR: ADC Overflow !");
		 ADC1->SR = ADC1->SR &~ADC_SR_OVR;
	}
	else
	{
		debug_line("ERROR: Unexpected ADC Interrupt (0x%08x) !", ADC1->SR);
	}
}
