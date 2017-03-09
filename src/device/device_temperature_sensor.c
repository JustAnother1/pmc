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

#include "device_buzzer.h"
#include "protocol.h"
#include "hal_cfg.h"
#include "hal_adc.h"
#include "hal_debug.h"

void dev_temperature_sensor_init(void)
{
    hal_adc_init();
}

uint_fast8_t dev_temperature_sensor_get_count(void)
{
    return hal_adc_get_amount();
}

uint_fast8_t dev_temperature_sensor_get_name(uint_fast8_t number, uint8_t *position, uint_fast8_t max_length)
{
    return hal_adc_get_name(number, position, max_length);
}

uint_fast8_t dev_temperature_sensor_get_status(uint_fast8_t number)
{
    if(number < hal_adc_get_amount())
    {
        return DEVICE_STATUS_ACTIVE;
    }
    else
    {
        return DEVICE_STATUS_FAULT;
    }
}

uint_fast16_t dev_temperature_sensor_get_temperature(uint_fast8_t number)
{
    return hal_adc_get_value(number);
}

#ifdef DEBUG_ACTIVE

void dev_temperature_sensor_print_status(uint_fast8_t number)
{
    uint_fast8_t name_length;
    uint_fast16_t temperature;
    uint_fast16_t adc_raw;
    uint8_t nameBuf[20];
    name_length = hal_adc_get_name(number, &(nameBuf[0]), 20);
    nameBuf[name_length] = 0;
    adc_raw = hal_adc_get_raw_value(number);
    temperature = hal_adc_get_value(number);
    debug_line(STR("%d: %s = %4d.%01d°C (raw =0x%04x)"), number, &(nameBuf[0]), temperature/10, temperature%10, adc_raw);
}

#endif // debug

// end of File
