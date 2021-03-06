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
#include "hal_debug.h"
#include "debug.h"
#ifdef WATCHDOG_ACTIVE
#include "hal_watchdog.h"
#endif
#include "hal_cpu.h"
#include "hal_din.h"
#include "hal_led.h"
#include "hal_time.h"
#include "hal_cfg.h"
#include "hal_power.h"
#include "end_stop_handling.h"
#include "step.h"


int main (void)
{
    bool on = false;
#ifdef WATCHDOG_ACTIVE
    watchdog_init();
#endif
    // initialize Hardware Abstraction Layer
    hal_cpu_init_hal();
    hal_init_leds();
    hal_time_init();
#ifdef DEBUG_ACTIVE
    debug_init();
#endif
    // initialize Pacemaker Client Firmware
    hal_init_power();
    hal_power_on_5V();
    hal_power_on_12V(); // Fans are on 12V ! So for safety it is critical to have 12V on all the time !
    hal_power_on_HighVoltage();
    events_init();
    fw_cfg_init();
    dev_temperature_sensor_init();
    dev_heater_init();
    dev_buzzer_init();
    dev_pwm_init();
    dev_input_init();
    dev_output_init();
    cmd_queue_init();
    com_init();
    dev_stepper_init();
    endStopHandling_init();
#ifdef DEBUG_ACTIVE
    debug_line(STR("Reached end of initialization!"));
#endif
    for(;;)
    {
        hal_set_debug_led(on);
        on = !on;
        com_tick();
        cmd_queue_tick();
#ifdef DEBUG_ACTIVE
        debug_tick();
#endif
        hal_cpu_tick();
#ifdef WATCHDOG_ACTIVE
        watchdog_tick();
#endif
#ifdef POLL_STEP_BUFFER_REFILL
        refill_step_buffer();
#endif
#ifdef POLL_END_STOPS
        hal_din_tick();
#endif
    }
    return 0;
}

// end of File
