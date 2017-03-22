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

#include "hal_i2c.h"

void hal_init_i2c(void)
{

}

void hal_print_i2c_configuration(void)
{

}

bool hal_do_i2c_transaction(bool read,
                            uint_fast8_t i2cAddress,
                            uint_fast8_t dataAddress,
                            uint8_t*     data,
                            uint_fast8_t num_bytes)
{
	// TODO
	(void) read;
	(void) i2cAddress;
	(void) dataAddress;
	(void) data;
	(void) num_bytes;
    return false;
}

void hal_start_i2c_transaction(bool read,
                               uint_fast8_t i2cAddress,
                               uint_fast8_t dataAddress,
                               uint8_t*     data,
                               uint_fast8_t num_bytes )
{
	// TODO
	(void) read;
	(void) i2cAddress;
	(void) dataAddress;
	(void) data;
	(void) num_bytes;
}

bool hal_i2c_is_idle(void)
{
    return true;
}


