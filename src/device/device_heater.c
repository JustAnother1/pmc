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
#include "device_buzzer.h"
#include "protocol.h"
#include "hal_adc.h"
#include "hal_pwm.h"
#include "hal_cfg.h"
#include "hal_cpu.h"
#include "hal_debug.h"
#include "hal_time.h"
#include "com.h"

#define INVALID_SENSOR 255
#define HALF_BANG_BANG_HYSTERESIS_DEG_C_DIV_10  10

void TemperatureControlTick(void);

#ifdef DEBUG_ACTIVE
static uint32_t count = 0;
#endif // debug

static uint32_t max_pwm = 0xffff;
static uint32_t min_pwm = 0;
static float last_iTerm = 0;
static uint_fast16_t last_temperature_is = 0;

static uint_fast8_t temperature_sensors[NUMBER_OF_HEATERS];
volatile uint_fast16_t target_temperature[NUMBER_OF_HEATERS];
static uint_fast16_t cur_pwm[NUMBER_OF_HEATERS];

typedef uint_fast16_t (*RegulatorFkt)(uint_fast16_t temperature_should, uint_fast16_t temperature_is, uint_fast16_t curPwm);
static RegulatorFkt regulators[NUMBER_OF_HEATERS];

uint_fast16_t BangBangRegulator(uint_fast16_t temperature_should, uint_fast16_t temperature_is, uint_fast16_t curPwm);
uint_fast16_t PidRegulator(uint_fast16_t temperature_should, uint_fast16_t temperature_is, uint_fast16_t curPwm);


void dev_heater_init(void)
{
    int i;
    for(i = 0; i < NUMBER_OF_HEATERS; i++)
    {
        temperature_sensors[i] = INVALID_SENSOR;
        target_temperature[i] = 0;
        cur_pwm[i] = 0;
        regulators[i] = PidRegulator;
    }
    hal_cpu_add_ms_tick_function_cycle(TemperatureControlTick, 25);
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

bool dev_heater_set_target_temperature(uint_fast8_t number, uint_fast16_t target_Temperature)
{
    if(number < NUMBER_OF_HEATERS)
    {
        target_temperature[number] = target_Temperature;
        return true;
    }
    else
    {
        // - temperature for a Heater that we do not have
        return false;
    }
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
        if((0 != target_temperature[i]) && (INVALID_SENSOR != temperature_sensors[i]))
        {
            uint_fast16_t newPwmValue;
            // Read Temperature
            uint_fast16_t curTemp = hal_adc_get_value(temperature_sensors[i]);
            if((0 == curTemp) || (0xffff == curTemp))
            {
                // Temperature Sensor error
                debug_line("ERROR: Temperature Sensor %d !", temperature_sensors[i]);
                hal_pwm_set_on_time(i, 0); // OFF
            }
            else
            {
                // compute next PWM setting
                if(NULL != regulators[i])
                {
                    newPwmValue = regulators[i](target_temperature[i], curTemp, cur_pwm[i]);
                    if(newPwmValue != cur_pwm[i])
                    {
                        hal_pwm_set_on_time(i, newPwmValue);
                        cur_pwm[i] = newPwmValue;
                    }
                    // else already correct PWM set.
                }
                else
                {
                    hal_pwm_set_on_time(i, 0); // OFF
                    debug_line("ERROR: Regulator %d NULL !", i);
                }
            }
        }
    }
}

uint_fast16_t BangBangRegulator(uint_fast16_t temperature_should, uint_fast16_t temperature_is, uint_fast16_t curPwm)
{
    if(temperature_is > temperature_should + HALF_BANG_BANG_HYSTERESIS_DEG_C_DIV_10)
    {
        // Set new PWM Mode - completely off
        return 0;
    }
    else if(temperature_is < temperature_should - HALF_BANG_BANG_HYSTERESIS_DEG_C_DIV_10)
    {
        // Set new PWM Mode - Full on
        return 0xffff;
    }
    else
    {
        // In hysteresis - no change
        return curPwm;
    }
}

uint_fast16_t PidRegulator(uint_fast16_t temperature_should, uint_fast16_t temperature_is, uint_fast16_t curPwm)
{
    float kp = 2086;
    float ki = 0;
    float kd = 0;
    float nextPwm;


    int_fast16_t error = temperature_should - temperature_is;

    float new_I = last_iTerm + (error * ki);
    if(new_I > max_pwm)
    {
        new_I = max_pwm;
    }
    else if(new_I < 0.0)
    {
        new_I = 0;
    }

    int_fast16_t temperatureChange = (temperature_is - last_temperature_is);

    // calculate the PID output
    nextPwm = (kp * error) + new_I - (kd * temperatureChange);

    if (nextPwm >= max_pwm)
    {
        nextPwm = max_pwm;
    }
    else if(nextPwm < min_pwm)
    {
        nextPwm = min_pwm;
    }
    else
    {
        last_iTerm = new_I; // Only update I term when output is not saturated.
    }

    last_temperature_is = temperature_is;
    return (uint_fast16_t)nextPwm;
}

#ifdef DEBUG_ACTIVE

void reportTemperature(void)
{
    count++;
    if(100 == count)
    {
        debug_line("time %d temp %d pwm %d", hal_cpu_get_ms_tick(), hal_adc_get_value(0), cur_pwm[0]);
        count = 0;
    }
}

#endif

/*
void curTest(int value)
{
    debug_line("Found Value %d !", value);
    // nozzle = 1, 5 ; Bed = 0, 0
    dev_heater_set_temperature_sensor(0, 0);
    // Nozzle = 1; Bed = 0
    dev_heater_set_target_temperature(0, value * 10);
    debug_line("Target Temperature %d !", target_temperature[0]);
    count = 0;
    hal_cpu_add_ms_tick_function(reportTemperature);
}
*/

// end of File
