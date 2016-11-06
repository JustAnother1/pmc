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
#include "hal_i2c.h"

void fw_cfg_init(void)
{
    hal_init_i2c();
}

void fw_cfg_get_value_property(uint_fast8_t idx)
{

}

void fw_cfg_get_next_value(uint_fast8_t last_value_idx)
{

}

void fw_cfg_get_value_of(uint_fast8_t parameterLength)
{

}

void fw_cfg_set_value_to(uint_fast8_t parameterLength)
{

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

/*
 *     bool hal_do_i2c_transaction(bool read,
                                uint_fast8_t i2cAddress,
                                uint_fast8_t dataAddress,
                                uint8_t*     data,
                                uint_fast8_t num_bytes );
 */


/* write
void curTest(int value)
{
    uint8_t data[3] = {23,24,25};
    debug_line("Found Value %d !", value);
    if(true == hal_do_i2c_transaction(false, 0xAC, 0, &data[0], 3))
    {
        debug_line("success!");
    }
    else
    {
        debug_line("failed!");
    }
}*/


// read
void curTest(int value)
{
    uint8_t data[3] = {0,0,0};
    debug_line("Found Value %d !", value);
    if(true == hal_do_i2c_transaction(true, 0xAC, 0, &data[0], 3))
    {
        debug_line("read: 0x%02x, 0x%02x, 0x%02x !", data[0], data[1], data[2]);
        debug_line("success!");
    }
    else
    {
        debug_line("failed!");
    }
}



// end of File
