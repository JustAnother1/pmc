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

#ifndef DEVICE_HEATER_H_
#define DEVICE_HEATER_H_

#include <inttypes.h>
#include <stdbool.h>

void dev_heater_init(void);
uint_fast8_t dev_heater_get_count(void);
uint_fast8_t dev_heater_get_name(uint_fast8_t number, uint8_t *position);
uint_fast8_t dev_heater_get_status(uint_fast8_t number);
void dev_heater_get_configuration(uint_fast8_t number);
void dev_heater_set_temperature_sensor(uint_fast8_t number, uint_fast8_t sensor_number);
bool dev_heater_set_target_temperature(uint_fast8_t number, uint_fast16_t target_temperature);
uint_fast16_t dev_heater_get_temperature(uint_fast8_t number);

#endif /* DEVICE_HEATER_H_ */
