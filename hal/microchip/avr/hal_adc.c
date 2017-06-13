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

#include "hal_adc.h"
#include "hal_cfg.h"
#include "board_cfg.h"
#include "log.h"
#include "hal_debug.h"
#include <math.h>
#include <avr/io.h>


#define VCC_OF_ADC            5.0
#define SERIES_RESISTOR      4700
#define THERMISTOR_R_AT_25 106300
#define HIGHES_ADC_VALUE     1023

typedef uint_fast16_t (*ADCTicksToDegCFkt)(uint16_t DR, uint_fast8_t number);

static uint_fast16_t NoConverter(uint16_t DR, uint_fast8_t number);
static uint_fast16_t SteinhartHartBOnlyConverter(uint16_t DR, uint_fast8_t number);


static ADCTicksToDegCFkt converters[ADC_NUM_PINS];
static float converter_parameter[ADC_NUM_PINS]; // Steinhard Hart B
static uint_fast32_t converter_reference[ADC_NUM_PINS]; // Thermistor Resistance at 25°C


void hal_adc_init(void)
{
#if ADC_NUM_PINS > 0
    converters[0] = SteinhartHartBOnlyConverter;
    converter_parameter[0] = 3800.5; // TODO: Configuration
    converter_reference[0] = 106300; // TODO: Configuration
#endif
#if ADC_NUM_PINS > 1
    converters[1] = NoConverter;
    converter_parameter[1] = 0.0;
    converter_reference[1] = 0;
#endif
#if ADC_NUM_PINS > 2
    converters[2] = NoConverter;
    converter_parameter[2] = 0.0;
    converter_reference[2] = 0;
#endif
#if ADC_NUM_PINS > 3
    converters[3] = NoConverter;
    converter_parameter[3] = 0.0;
    converter_reference[3] = 0;
#endif
#if ADC_NUM_PINS > 4
    converters[4] = SteinhartHartBOnlyConverter;
    converter_parameter[4] = 4203.1;
    converter_reference[4] = 110700;
#endif

    ADCSRA = ADC_ADCSRA;
    DIDR0  = ADC_DIDR0;
    DIDR1  = ADC_DIDR1;
}

uint_fast8_t hal_adc_get_amount(void)
{
    return ADC_NUM_PINS;
}

uint_fast16_t hal_adc_get_value(uint_fast8_t device)
{
    if(device < ADC_NUM_PINS)
    {
        return converters[device](hal_adc_get_raw_value(device), device);
    }
    else
    {
        return 0;
    }
}
uint_fast16_t hal_adc_get_raw_value(uint_fast8_t device)
{
    if(device < ADC_NUM_PINS)
    {
        // Configure channel
        switch(device)
        {
#if ADC_NUM_PINS > 0
        case 0:
            ADMUX = ADC_ADMUX_0;
            ADCSRB =  ADC_ADCSRB_0;
            break;
#endif
#if ADC_NUM_PINS > 1
        case 1:
            ADMUX = ADC_ADMUX_1;
            ADCSRB =  ADC_ADCSRB_1;
            break;
#endif
#if ADC_NUM_PINS > 2
        case 2:
            ADMUX = ADC_ADMUX_2;
            ADCSRB =  ADC_ADCSRB_2;
            break;
#endif
#if ADC_NUM_PINS > 3
        case 3:
            ADMUX = ADC_ADMUX_3;
            ADCSRB =  ADC_ADCSRB_3;
            break;
#endif
#if ADC_NUM_PINS > 4
        case 4:
            ADMUX = ADC_ADMUX_4;
            ADCSRB =  ADC_ADCSRB_4;
            break;
#endif
        }
        // start a measurement
        ADCSRA |= 0x40;
        // wait for result
        while(0 != (0x40 & ADCSRA))
        {
            ;
        }
        // return result
        return ADC;
    }
    else
    {
        return 0;
    }
}

uint_fast8_t hal_adc_get_name(uint_fast8_t device, uint8_t *position, uint_fast8_t max_length)
{
    switch(device)
    {
#if ADC_NUM_PINS > 0
    case  0: return strlcpy_P((char *)position, STR(ADC_0_NAME), max_length);
#endif
#if ADC_NUM_PINS > 1
    case  1: return strlcpy_P((char *)position, STR(ADC_1_NAME), max_length);
#endif
#if ADC_NUM_PINS > 2
    case  2: return strlcpy_P((char *)position, STR(ADC_2_NAME), max_length);
#endif
#if ADC_NUM_PINS > 3
    case  3: return strlcpy_P((char *)position, STR(ADC_3_NAME), max_length);
#endif
#if ADC_NUM_PINS > 4
    case  4: return strlcpy_P((char *)position, STR(ADC_4_NAME), max_length);
#endif
    default:
        return 0;
    }
}

void hal_print_configuration_adc(void)
{

}

static uint_fast16_t SteinhartHartBOnlyConverter(uint16_t DR, uint_fast8_t number)
{
    uint_fast16_t ires;
    // debug_line(STR("ADC value: %d"), DR);
    float res = VCC_OF_ADC/HIGHES_ADC_VALUE * DR;
    // debug_line(STR("Vadc: %f"), res);
    res = SERIES_RESISTOR/((VCC_OF_ADC/res) -1);
    //debug_line(STR("Rthermistor: %f"), res);
    res = res/THERMISTOR_R_AT_25;
    // debug_line(STR("before Log: %f"), res);
    // The logf() doesn't work. But the avrlib's log works,..
    res = log(res);
    // debug_line(STR("after Log: %f"), res);
    // STEINHART_HART_B is the parameter
    res = res / converter_parameter[number] + (1.0/(25+273.15));
    // debug_line(STR("after B: %f"), res);
    res = 1/res - 273.15;
    // debug_line(STR("Temperature: %f"), res);
    ires = res * 10;
    return (uint_fast16_t)0xffff & ires;
}

static uint_fast16_t NoConverter(uint16_t DR, uint_fast8_t number)
{
    (void)number;
    return (uint_fast16_t)0xffff & DR;
}
