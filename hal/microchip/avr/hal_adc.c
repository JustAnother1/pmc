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
#include "board_cfg.h"
#include "log.h"

typedef uint_fast16_t (*ADCTicksToDegCFkt)(uint16_t DR);

static uint_fast16_t NoConverter(uint16_t DR);
static uint_fast16_t SteinhartHartBOnlyConverter(uint16_t DR);


static ADCTicksToDegCFkt converters[ADC_NUM_PINS];



void hal_adc_init(void)
{
#if ADC_NUM_PINS > 0
    converters[0] = SteinhartHartBOnlyConverter;
#endif
#if ADC_NUM_PINS > 1
    converters[1] = SteinhartHartBOnlyConverter;
#endif
#if ADC_NUM_PINS > 2
    converters[2] = SteinhartHartBOnlyConverter;
#endif
#if ADC_NUM_PINS > 3
    converters[3] = SteinhartHartBOnlyConverter;
#endif
#if ADC_NUM_PINS > 4
    converters[4] = SteinhartHartBOnlyConverter;
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
    case  0: return strlcpy_P((char *)position, ADC_0_NAME, max_length);
#endif
#if ADC_NUM_PINS > 1
    case  1: return strlcpy_P((char *)position, ADC_1_NAME, max_length);
#endif
#if ADC_NUM_PINS > 2
    case  2: return strlcpy_P((char *)position, ADC_2_NAME, max_length);
#endif
#if ADC_NUM_PINS > 3
    case  3: return strlcpy_P((char *)position, ADC_3_NAME, max_length);
#endif
#if ADC_NUM_PINS > 4
    case  4: return strlcpy_P((char *)position, ADC_4_NAME, max_length);
#endif
    default:
        return 0;
    }
}

void hal_print_configuration_adc(void)
{

}

#define VCC_OF_ADC            5.0
#define SERIES_RESISTOR      4700
#define THERMISTOR_R_AT_25 100000
#define STEINHART_HART_B     3974

static uint_fast16_t SteinhartHartBOnlyConverter(uint16_t DR)
{
    uint_fast16_t ires;
    // debug_line(STR("ADC value: %d"), DR);
    float res = VCC_OF_ADC/4095 * DR;
    // debug_line(STR("Vadc: %f"), res);
    res = SERIES_RESISTOR/((VCC_OF_ADC/res) -1);
    // debug_line(STR("Rthermistor: %f"), res);
    res = logf(res/THERMISTOR_R_AT_25);
    res = res / STEINHART_HART_B + (1.0/(25+273.15));
    res = 1/res - 273.15;
    // debug_line(STR("Temperature: %f"), res);
    ires = res * 10;
    return (uint_fast16_t)0xffff & ires;
}

static uint_fast16_t NoConverter(uint16_t DR)
{
    return (uint_fast16_t)0xffff & DR;
}
