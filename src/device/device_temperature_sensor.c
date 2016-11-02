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

uint_fast8_t dev_temperature_sensor_get_name(uint_fast8_t number, uint8_t *position)
{
    return hal_adc_get_name(number, position);
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

void dev_temperature_sensor_print_status(void)
{
    uint_fast16_t temperature;
    // unsigned char nameBuf[20];
    int i;
    for(i = 0; i < hal_adc_get_amount(); i++)
    {
        // hal_adc_get_name(i, &(nameBuf[0]));
        temperature = hal_adc_get_value(i);
        //debug_line("%d: %s = %d.%01d°C", i, &(nameBuf[0]), temperature/10, temperature%10);  TODO name not working
        debug_line("%d: %d.%01d°C", i, temperature/10, temperature%10);
    }
}

// end of File
