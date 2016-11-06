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

#ifndef FW_CFG_H_
#define FW_CFG_H_

#include <inttypes.h>

#define SETTING_SERIAL_NUMBER   0
#define SETTING_GIVEN_NAME      1
#define LAST_SETTING_IDX        1

uint_fast8_t fw_cfg_read_setting_to(uint_fast8_t setting, uint8_t *position);
void fw_cfg_init(void);
void fw_cfg_get_value_property(uint_fast8_t idx);
void fw_cfg_get_next_value(uint_fast8_t last_value_idx);
void fw_cfg_get_value_of(uint_fast8_t parameterLength);
void fw_cfg_set_value_to(uint_fast8_t parameterLength);

#endif /* FW_CFG_H_ */
