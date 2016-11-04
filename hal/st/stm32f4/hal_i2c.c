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
#include "board_cfg.h"
#include "st_gpio.h"
#include "st_rcc.h"
#include "st_i2c.h"

#define TIMEOUT_MS   500

static bool idle;
static bool successfully_received;

void hal_init_i2c(void)
{
    idle = true; // no activities yet, the bus is ours!
    successfully_received = false;  // not yet
}

void hal_print_i2c_configuration(void)
{
    debug_line("Configuration of I2C :");
    // Clock
    debug_line("RCC->AHB1ENR  = 0x%08x", RCC->AHB1ENR);
    debug_line("RCC->APB1ENR  = 0x%08x", RCC->APB1ENR);
    debug_line("RCC->APB2ENR  = 0x%08x", RCC->APB2ENR);
    // I2C
    debug_line("I2C_0->CR1      = 0x%08x", I2C_0->CR1);
    debug_line("I2C_0->CR2      = 0x%08x", I2C_0->CR2);
    debug_line("I2C_0->OAR1     = 0x%08x", I2C_0->OAR1);
    debug_line("I2C_0->OAR2     = 0x%08x", I2C_0->OAR2);
    debug_line("I2C_0->DR       = 0x%08x", I2C_0->DR);
    debug_line("I2C_0->SR1      = 0x%08x", I2C_0->SR1);
    debug_line("I2C_0->SR2      = 0x%08x", I2C_0->SR2);
    debug_line("I2C_0->CCR      = 0x%08x", I2C_0->CCR);
    debug_line("I2C_0->TRISE    = 0x%08x", I2C_0->TRISE);
    debug_line("I2C_0->FLTR     = 0x%08x", I2C_0->FLTR);
    // GPIO
    debug_line("SCL Pin:");
    print_gpio_configuration(I2C_0_SCL_GPIO_PORT);
    debug_line("SDA Pin:");
    print_gpio_configuration(I2C_0_SDA_GPIO_PORT);
}

bool hal_do_i2c_transaction(bool read,
                            uint_fast8_t i2cAddress,
                            uint_fast8_t dataAddress,
                            uint8_t*     data,
                            uint_fast8_t num_bytes)
{
    uint32_t curTime = hal_time_get_ms_tick();
    uint32_t endTime = curTime + TIMEOUT_MS;
    while((false == idle) && (endTime > curTime))
    {
        curTime = hal_time_get_ms_tick(); // wait until we can send the data out
    }
    if(endTime == curTime)
    {
        debug_line("I2C not Idle !");
        return false;
    }

    hal_start_i2c_transaction(read, i2cAddress, dataAddress, data, num_bytes);

    curTime = hal_time_get_ms_tick();
    endTime = curTime + TIMEOUT_MS;
    while((false == idle) && (endTime > curTime))
    {
        curTime = hal_time_get_ms_tick(); // wait until we have the data back
    }
    if(endTime == curTime)
    {
        debug_line("I2C Transaction never finished!");
        return false;
    }
    return successfully_received;
}

void hal_start_i2c_transaction(bool read,
                               uint_fast8_t i2cAddress,
                               uint_fast8_t dataAddress,
                               uint8_t*     data,
                               uint_fast8_t num_bytes )
{
    return false;
}

bool hal_i2c_is_idle(void)
{
    return idle;
}


