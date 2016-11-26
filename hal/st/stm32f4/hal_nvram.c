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

#include "hal_debug.h"
#include "hal_i2c.h"
#include "hal_nvram.h"

static uint_fast8_t get_i2c_device_address_for(uint32_t addr);
static uint_fast8_t get_i2c_data_address_for(uint32_t addr);

void hal_nvram_init(void)
{
    // TODO
    hal_init_i2c();
}

bool hal_nvram_getString(uint32_t addr, char* buf, int maxLength)
{
    if(true == hal_do_i2c_transaction(
            true, // read and not write
            get_i2c_device_address_for(addr), // read from this i2C device
            get_i2c_data_address_for(addr), // read from this address in the device
            (uint8_t* )buf, // read the data into this buffer
            maxLength)) // read not more than this number of bytes
    {
        // -> OK
        return true;
    }
    else
    {
        debug_line("ERROR: I2C read failed !");
        return false;
    }
}

void hal_nvram_setString(uint32_t addr, char* buf, int length)
{
    //TODO alignment !
    if(true == hal_do_i2c_transaction(
            false, // write and not read
            get_i2c_device_address_for(addr), // write to this device
            get_i2c_data_address_for(addr), // write to this address in the device
            (uint8_t* )buf, // the data to write
            length)) // number of Bytes to write
    {
        // OK
    }
    else
    {
        debug_line("ERROR: I2C write failed !");
    }
}

bool hal_nvram_getUInt32Value(uint32_t addr, uint32_t* value)
{
    if(true == hal_do_i2c_transaction(
            true, // read and not write
            get_i2c_device_address_for(addr), // read from this i2C device
            get_i2c_data_address_for(addr), // read from this address in the device
            (uint8_t* )value,  // read the data into this buffer
            4)) // read 4 bytes
    {
        // -> OK
        return true;
    }
    else
    {
        debug_line("ERROR: I2C read failed !");
        return false;
    }
}

void hal_nvram_setUInt32Value(uint32_t addr, uint32_t value)
{
    //TODO alignment !
    if(true == hal_do_i2c_transaction(
            false, // write and not read
            get_i2c_device_address_for(addr), // write to this device
            get_i2c_data_address_for(addr), // write to this address in the device
            (uint8_t* )&value, // the data to write
            4)) // 4 Bytes to write
    {
        // OK
    }
    else
    {
        debug_line("ERROR: I2C write failed !");
    }
}

bool hal_nvram_getUInt16Value(uint32_t addr, uint16_t* value)
{
    if(true == hal_do_i2c_transaction(
            true, // read and not write
            get_i2c_device_address_for(addr), // read from this i2C device
            get_i2c_data_address_for(addr), // read from this address in the device
            (uint8_t* )value,  // read the data into this buffer
            2)) // read 2 bytes
    {
        // -> OK
        return true;
    }
    else
    {
        debug_line("ERROR: I2C read failed !");
        return false;
    }
}

void hal_nvram_setUInt16Value(uint32_t addr, uint16_t value)
{
    //TODO alignment !
    if(true == hal_do_i2c_transaction(
            false, // write and not read
            get_i2c_device_address_for(addr), // write to this device
            get_i2c_data_address_for(addr), // write to this address in the device
            (uint8_t* )&value, // the data to write
            2)) // 2 Bytes to write
    {
        // OK
    }
    else
    {
        debug_line("ERROR: I2C write failed !");
    }
}

bool hal_nvram_getUInt8Value(uint32_t addr, uint8_t* value)
{
    if(true == hal_do_i2c_transaction(
            true, // read and not write
            get_i2c_device_address_for(addr), // read from this i2C device
            get_i2c_data_address_for(addr), // read from this address in the device
            value,  // read the data into this buffer
            1)) // read 1 bytes
    {
        // -> OK
        return true;
    }
    else
    {
        debug_line("ERROR: I2C read failed !");
        return false;
    }
}

void hal_nvram_setUInt8Value(uint32_t addr, uint8_t value)
{
    if(true == hal_do_i2c_transaction(
            false, // write and not read
            get_i2c_device_address_for(addr), // write to this device
            get_i2c_data_address_for(addr), // write to this address in the device
            &value, // the data to write
            1)) // 1 Bytes to write
    {
        // OK
    }
    else
    {
        debug_line("ERROR: I2C write failed !");
    }
}

bool hal_nvram_getfloat(uint32_t addr, float* value)
{
    if(true == hal_do_i2c_transaction(
            true, // read and not write
            get_i2c_device_address_for(addr), // read from this i2C device
            get_i2c_data_address_for(addr), // read from this address in the device
            (uint8_t* )value,  // read the data into this buffer
            4)) // read 4 bytes
    {
        // -> OK
        return true;
    }
    else
    {
        debug_line("ERROR: I2C read failed !");
        return false;
    }
}

void hal_nvram_setfloat(uint32_t addr, float value)
{
    //TODO alignment !
    if(true == hal_do_i2c_transaction(
            false, // write and not read
            get_i2c_device_address_for(addr), // write to this device
            get_i2c_data_address_for(addr), // write to this address in the device
            (uint8_t* )&value, // the data to write
            4)) // 4 Bytes to write
    {
        // OK
    }
    else
    {
        debug_line("ERROR: I2C write failed !");
    }
}

static uint_fast8_t get_i2c_device_address_for(uint32_t addr)
{
    // TODO
    return 0xAC;
}

static uint_fast8_t get_i2c_data_address_for(uint32_t addr)
{
    // TODO
    return 0xff & addr;
}
