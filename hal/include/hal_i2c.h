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

#ifndef HAL_I2C_H_
#define HAL_I2C_H_

#include <inttypes.h>
#include <stdbool.h>

// I2C Bus

void hal_init_i2c(void);
void hal_print_i2c_configuration(void);

bool hal_do_i2c_transaction(bool read,
                            uint_fast8_t i2cAddress,
                            uint_fast8_t dataAddress,
                            uint8_t*     data,
                            uint_fast8_t num_bytes );

bool hal_start_i2c_transaction(bool read,
                               uint_fast8_t i2cAddress,
                               uint_fast8_t dataAddress,
                               uint8_t*     data,
                               uint_fast8_t num_bytes );

bool hal_i2c_is_idle(void);

#endif /* HAL_I2C_H_ */
