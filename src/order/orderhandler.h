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

#ifndef ORDERHANDLER_H_
#define ORDERHANDLER_H_

void oh_handle_request_information(uint_fast8_t parameter);
void oh_handle_request_device_name(uint_fast8_t device_type, uint_fast8_t device_number);
void oh_handle_request_device_count(uint_fast8_t device_type);
void oh_handle_request_device_status(uint_fast8_t device_type, uint_fast8_t device_number);
void oh_handle_set_pwm_output_value(uint_fast8_t device_type, uint_fast8_t device_number, uint_fast16_t pwm);
void oh_handle_set_output_tone(uint_fast8_t device_type, uint_fast8_t device_number, uint_fast16_t frequency);
void oh_handle_request_temperature_reading(uint_fast8_t parameter_length);
void oh_handle_request_state_of_switch(uint_fast8_t parameter_length);
void oh_handle_switch_an_output(uint_fast8_t parameter_length);

#endif /* ORDERHANDLER_H_ */
