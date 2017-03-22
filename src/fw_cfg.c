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

#include "fw_cfg.h"
#include "hal_nvram.h"

void fw_cfg_init(void)
{
    hal_nvram_init();
    // read version from EEPROM
    // if incorrect -> write default values
}

void fw_cfg_get_value_property(uint_fast8_t idx)
{
	// TODO
	(void) idx;
}

void fw_cfg_get_next_value(uint_fast8_t last_value_idx)
{
	// TODO
	(void) last_value_idx;
}

void fw_cfg_get_value_of(uint_fast8_t parameterLength)
{
	// TODO
	(void) parameterLength;
}

void fw_cfg_set_value_to(uint_fast8_t parameterLength)
{
	// TODO
	(void) parameterLength;
}

uint_fast8_t fw_cfg_read_setting_to(uint_fast8_t setting, uint8_t *position)
{
    switch(setting)
    {
    case SETTING_SERIAL_NUMBER:
        *position = '0';
        position++;
        *position = '8';
        position++;
        *position = '1';
        position++;
        *position = '5';
        return 4;

    case SETTING_GIVEN_NAME:
        *position = 'H';
        position++;
        *position = 'u';
        position++;
        *position = 'g';
        position++;
        *position = 'o';
        return 4;

    default:
        return 0;
    }
}

// end of File
