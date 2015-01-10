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

#include "com.h"
#include "led.h"
#include "error.h"
#include "device_buzzer.h"
#include "device_heater.h"
#include "device_input.h"
#include "device_output.h"
#include "device_pwm.h"
#include "device_stepper.h"
#include "device_temperature_sensor.h"
#include "events.h"
#include "fw_cfg.h"
#include "command_queue.h"
#include "hal_watchdog.h"
#include "hal_cpu.h"
#include "hal_led.h"
#include "hal_time.h"


int main (void)
{
#ifdef WATCHDOG_ACTIVE
    watchdog_init();
#endif
    hal_cpu_init_hal();
    hal_time_init();
    hal_led_init();
    hal_cpu_complete_init();
    events_init();
    fw_cfg_init();
    led_init();
    dev_buzzer_init();
    dev_stepper_init();
    dev_heater_init();
    dev_temperature_sensor_init();
    dev_pwm_init();
    dev_input_init();
    dev_output_init();
    cmd_queue_init();
    com_init();
    for(;;)
    {
        com_tick();
        cmd_queue_tick();
#ifdef WATCHDOG_ACTIVE
         watchdog_tick();
#endif
        led_tick();
    }
    return 0;
}

// end of File
