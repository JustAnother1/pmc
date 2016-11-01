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
#include "hal_cpu.h"
#include "com.h"

#define INVALID_SENSOR 255
#define HALF_BANG_BANG_HYSTERESIS_DEG_C  1

void TemperatureControlTick(void);

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
    hal_cpu_add_ms_tick_function(TemperatureControlTick);
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
    // first Byte is 0x00 - we do not have an invisible internal Temperature sensor
    // the second byte is the heater Number or 0xff
    if(number < NUMBER_OF_HEATERS)
    {
        com_send_ok_response_with_two_byte_parameter(0, temperature_sensors[number]);
    }
    else
    {
        com_send_generic_application_error_response(GENERIC_ERROR_BAD_PARAMETER_VALUE);
    }
}

void dev_heater_set_temperature_sensor(uint_fast8_t number, uint_fast8_t sensor_Number)
{
    if(number < NUMBER_OF_HEATERS)
    {
        temperature_sensors[number] = sensor_Number;
    }
    // else Ignore Temperature sensor for not existing heater
}

void dev_heater_set_target_temperature(uint_fast8_t number, uint_fast16_t target_Temperature)
{
    if(number < NUMBER_OF_HEATERS)
    {
        target_temperature[number] = target_Temperature;
    }
    // else - temperature for a Heater that we do not have
}

uint_fast16_t dev_heater_get_temperature(uint_fast8_t number)
{
    if(number < NUMBER_OF_HEATERS)
    {
        if(INVALID_SENSOR == temperature_sensors[number])
        {
            return 0x7FFF;
        }
        else
        {
            return hal_adc_get_value(temperature_sensors[number]);
        }
    }
    else
    {
        return 0x7FFF;
    }
}

void TemperatureControlTick(void)
{
    int i;
    for(i = 0; i < NUMBER_OF_HEATERS; i++)
    {
        if(0 != target_temperature[i])
        {
            // Read Temperature
            uint_fast16_t curTemp = hal_adc_get_value(temperature_sensors[i]);
            // PID / Bang Bang
            // PID
            // TODO
            // Bang Bang
            if(curTemp > target_temperature[i] + HALF_BANG_BANG_HYSTERESIS_DEG_C)
            {
                // Set new PWM Mode - completely off
                hal_pwm_set_on_time(i, 0);
            }
            else if(curTemp <  target_temperature[i] - HALF_BANG_BANG_HYSTERESIS_DEG_C)
            {
                // Set new PWM Mode - Full on
                hal_pwm_set_on_time(i, 0xffff);
            }
        }
    }
}
/*
void curTest(int value)
{
    debug_line("Found Value %d !", value);
    dev_heater_set_temperature_sensor(1, 0);
    dev_heater_set_target_temperature(1, value);
}
*/
// end of File
