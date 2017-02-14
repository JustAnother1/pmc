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

#ifndef COM_H_
#define COM_H_

#include <inttypes.h>

void com_init(void);
void com_tick(void);
void com_send_stepper_error_event(uint_fast8_t cur_errors);
void com_send_generic_application_error_response(uint_fast8_t type);
void com_send_ok_response(void);
void com_send_ok_response_with_single_byte_parameter(uint_fast8_t parameter);
void com_send_ok_response_with_two_byte_parameter(uint_fast8_t parameter1, uint_fast8_t parameter2);
void com_send_ok_with_prefilled_parameter(uint_fast8_t parameter_length);
void com_send_debug_frame_with_filled_parameter(uint_fast8_t parameter_length);
void com_send_bad_crc(uint_fast8_t received, uint_fast8_t expected);
uint8_t *com_get_start_parameter(void);
uint_fast8_t com_get_parameter_byte(uint_fast8_t index);
void com_send_generic_application_error_response_with_comment(uint_fast8_t cause, uint_fast8_t message_index, uint_fast8_t value);
void com_send_order_specific_error_with_prefilled_parameter(uint_fast8_t parameter_length);
uint_fast8_t com_crc_a_byte(uint_fast8_t new_byte, uint_fast8_t old_crc);
uint_fast8_t com_get_host_timeout_sec(void);
void gotoStoppedMode(uint_fast8_t cause_for_stopped_mode, uint_fast8_t recovery_options_for_stopped_mode);
uint_fast8_t com_get_max_parameter_length(void);

#endif /* COM_H_ */
