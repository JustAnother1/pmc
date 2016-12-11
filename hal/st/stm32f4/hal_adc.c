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

#include <math.h>
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
#include "hal_spi.h"
#include "debug.h"
#include "hal_cpu.h"
#include "hal_i2c.h"

#define ADC_USE_DMA 0

// Temperatures measured with internal ADC = Pins + Chip Temperature
#define NUM_TEMPERATURES                      (ADC_NUM_PINS + 1)
// Temperatures measured by external chips ( Thermocouple, I2C Sensor)
#define NUM_EXTERNAL_TEMPERATURES             2
#define MAX_TRIES                             500
#define I2C_TEMP_POLL_MS                      240 // The I2C sensor measures only every 240ms
#define I2C_TEMPERATURE_SENSOR_DEVICE_ADDRESS 0x90
#define I2C_TEMPERATURE_SENSOR_DATA_ADDRESS   0


typedef uint_fast16_t (*ADCTicksToDegCFkt)(uint32_t DR);
static ADCTicksToDegCFkt converters[NUM_TEMPERATURES];

static bool start;
static bool spi_ok;
static int curDevice;
static uint16_t res_buf[NUM_TEMPERATURES + NUM_EXTERNAL_TEMPERATURES];
static uint32_t nextMeasurement;
static bool initialized = false;

static void aquireValues(void);

static uint_fast16_t NoConverter(uint32_t DR);
static uint_fast16_t InternalTempSensorConverter(uint32_t DR);
static uint_fast16_t SteinhartHartBOnlyConverter(uint32_t DR);

void hal_adc_init(void)
{
    if(true == initialized)
    {
        // initialize only once !
        return;
    }
    initialized = true;
    int i;
    for(i = 0; i < NUM_TEMPERATURES; i++)
    {
        converters[i] = NoConverter;
    }
    converters[0] = SteinhartHartBOnlyConverter;
    converters[4] = InternalTempSensorConverter;

    nextMeasurement = 0;

    hal_init_expansion_spi();
    hal_init_i2c();

    start = true;
    spi_ok = true;
    curDevice = 0;
    for(i = 0; i < (NUM_TEMPERATURES + NUM_EXTERNAL_TEMPERATURES); i++)
    {
        res_buf[i] = 7; // -> 0.7 °C ;-)
    }
#if 1 == ADC_USE_DMA
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
/*
    // I2C
    {
        // 0x80 in Register 3 sets the resolution to 16bit.
        uint8_t data[2] = {0x80, 0};
        if(true == hal_do_i2c_transaction(false, 0x90, 3, &data[0], 1))
        {
                // debug_line("success!");
        }
        else
        {
            debug_line("ERROR: I2C write failed!");
        }
    }
*/
    hal_cpu_add_ms_tick_function(aquireValues);
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
    return NUM_TEMPERATURES + NUM_EXTERNAL_TEMPERATURES;
}

static void aquireValues(void)
{
    static uint8_t receive_data[4];
    static uint8_t send_data[4];

    if(curDevice < NUM_TEMPERATURES)
    {
        if(true == start)
        {
            switch(curDevice)
            {
            case 0: ADC1->SQR3 = ADC_0_INPUT_NUM; break;
            case 1: ADC1->SQR3 = ADC_1_INPUT_NUM; break;
            case 2: ADC1->SQR3 = ADC_2_INPUT_NUM; break;
            case 3: ADC1->SQR3 = ADC_3_INPUT_NUM; break;
            default:
            case 4: ADC1->SQR3 = ADC_4_INPUT_NUM; break;
            }
            ADC1->CR2 = ADC_CR2_SWSTART | ADC_CR2_EOCS | ADC_CR2_ADON;
            start = false;
        }
        else
        {
            res_buf[curDevice] = converters[curDevice](ADC1->DR);
            start = true;
        }
    }
    else if(curDevice < (NUM_TEMPERATURES + NUM_EXTERNAL_TEMPERATURES))
    {
        switch(curDevice - NUM_TEMPERATURES)
        {
        case 0: // SPI - Thermocouple
            if(true == start)
            {
                hal_start_expansion_spi_transaction(&send_data[0], 4, &receive_data[0]);
                start = false;
            }
            else
            {
                uint32_t tempMeasured;
                if(true == spi_ok)
                {
                    if(1 == (receive_data[3] & 0x01))
                    {
                        debug_line("ERROR SPI Thermo: Open Circuit.");
                        spi_ok = false;
                    }
                    if(2 == (receive_data[3] & 0x02))
                    {
                        debug_line("ERROR SPI Thermo: Short to GND.");
                        spi_ok = false;
                    }
                    if(4 == (receive_data[3] & 0x04))
                    {
                        debug_line("ERROR SPI Thermo: Short to Vcc.");
                        spi_ok = false;
                    }
                    if(1 == (receive_data[1] & 0x01))
                    {
                        debug_line("ERROR SPI Thermo: Fault Bit.");
                        spi_ok = false;
                    }

                    tempMeasured = (receive_data[1]>>2) + (receive_data[0] << 6);
                    tempMeasured = tempMeasured * 10;
                    tempMeasured = tempMeasured / 4;

                    if(10000 < tempMeasured)  // 10,000 == 1,000°C -> much too high! -> something went wrong!
                    {
                        tempMeasured = 0;
                    }

                    res_buf[curDevice] = tempMeasured;
                }
                else
                {
                    if( (1 != (receive_data[3] & 0x01)) &&
                        (2 != (receive_data[3] & 0x02)) &&
                        (4 != (receive_data[3] & 0x04)) &&
                        (1 != (receive_data[1] & 0x01)) )
                    {
                        spi_ok = true;
                        tempMeasured = (receive_data[1]>>2) + (receive_data[0] << 6);
                        tempMeasured = tempMeasured * 10;
                        tempMeasured = tempMeasured / 4;

                        if(10000 < tempMeasured)// 10,000 == 1,000°C -> much too high! -> something went wrong!
                        {
                            tempMeasured = 0;
                        }

                        res_buf[curDevice] = tempMeasured;
                    }
                    // else - SPI still fails
                }
                start = true;
            }
            break;

        case 1: // I2C Temperature Sensor
            /* Does not work under stress
            if(nextMeasurement < hal_cpu_get_ms_tick())
            {
                uint8_t data[2];

                if(true == hal_do_i2c_transaction(true,
                        I2C_TEMPERATURE_SENSOR_DEVICE_ADDRESS,
                        I2C_TEMPERATURE_SENSOR_DATA_ADDRESS,
                        &data[0], 2) )
                {
                    uint32_t res;
                    res = (data[0] <<8) + data[1];
                    if(res > 32767)
                    {
                        // negative temperature
                        res_buf[curDevice] = 0; // I don't care if below zero !
                    }
                    else
                    {
                        // positive temperature
                        res_buf[curDevice] = (res/128) * 10; // 0.1°C resolution
                    }
                }
                else
                {
                    debug_line("ERROR: I2C Temp failed!");
                }
                nextMeasurement = hal_cpu_get_ms_tick() + I2C_TEMP_POLL_MS;
            }
            */
            break;

        default:
            // invalid device number
            debug_line("Invalid External Device %d (%d) !", curDevice, curDevice - NUM_TEMPERATURES);
            res_buf[curDevice] = 3;
            break;
        }
    }
    if(true == start)
    {
        curDevice++;
        if(curDevice > (NUM_TEMPERATURES + NUM_EXTERNAL_TEMPERATURES))
        {
            curDevice = 0;
        }
    }
}

uint_fast16_t hal_adc_get_value(uint_fast8_t device)
{
    if(device < (NUM_TEMPERATURES + NUM_EXTERNAL_TEMPERATURES))
    {
        return res_buf[device];
    }
    else
    {
        // invalid device number
        debug_line("Invalid Device %d !", device);
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
#if NUM_TEMPERATURES > 5
        case  5: return copy_string(ADC_5_NAME, position);
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

static uint_fast16_t InternalTempSensorConverter(uint32_t DR)
{
    uint32_t help = DR * 1000;
    help = help - 952087;
    help = help /341;
    return (uint_fast16_t)0xffff & help;
}

#define VCC_OF_ADC            3.0
#define SERIES_RESISTOR      4700
#define THERMISTOR_R_AT_25 100000
#define STEINHART_HART_B     3974

static uint_fast16_t SteinhartHartBOnlyConverter(uint32_t DR)
{
    int ires;
    // debug_line("ADC value: %d", DR);
    float res = VCC_OF_ADC/4095 * DR;
    // debug_line("Vadc: %f", res);
    res = SERIES_RESISTOR/((VCC_OF_ADC/res) -1);
    // debug_line("Rthermistor: %f", res);
    res = log(res/THERMISTOR_R_AT_25);
    res = res / STEINHART_HART_B + (1.0/(25+273.15));
    res = 1/res - 273.15;
    // debug_line("Temperature: %f", res);
    ires = res * 10;
    return (uint_fast16_t)0xffff & ires;
}

static uint_fast16_t NoConverter(uint32_t DR)
{
    return (uint_fast16_t)0xffff & DR;
}

