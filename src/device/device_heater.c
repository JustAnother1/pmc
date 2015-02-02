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
#include "hal_pwm.h"
#include "hal_cfg.h"

#define INVALID_SENSOR 253

static uint_fast8_t temperature_sensors[NUMBER_OF_HEATERS];
static uint_fast16_t target_temperature[NUMBER_OF_HEATERS];

void dev_heater_init(void)
{
    int i;
    for(i = 0; i < NUMBER_OF_HEATERS; i++)
    {
        temperature_sensors[i] = INVALID_SENSOR;
        target_temperature[i] = 0;
    }
}

uint_fast8_t dev_heater_get_count(void)
{
    return NUMBER_OF_HEATERS;
}

uint_fast8_t dev_heater_get_name(uint_fast8_t number, uint8_t *position)
{
    return hal_pwm_get_name(number, position);
}

uint_fast8_t dev_heater_get_status(uint_fast8_t number)
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

void dev_heater_get_configuration(uint_fast8_t number)
{
    // TODO
}

void dev_heater_set_temperature_sensor(uint_fast8_t number, uint_fast8_t sensor_Number)
{
    temperature_sensors[number] = sensor_Number;
}

void dev_heater_set_target_temperature(uint_fast8_t number, uint_fast16_t target_Temperature)
{
    target_temperature[number] = target_Temperature;
    // TODO
}

uint_fast16_t dev_heater_get_temperature(uint_fast8_t number)
{
    if(number < dev_heater_get_count())
    {
        if(INVALID_SENSOR == temperature_sensors[number])
        {
            return 0x7FFF;
        }
        else
        {
            return hal_adc_get_value(number);
        }
    }
    else
    {
        return 0x7FFF;
    }
}

// end of File
