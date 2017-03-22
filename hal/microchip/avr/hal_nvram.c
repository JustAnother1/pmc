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

#include "hal_nvram.h"


void hal_nvram_init(void)
{

}

bool hal_nvram_getString(uint32_t addr, char* buf, int maxLength)
{
	// TODO
	(void) addr;
	(void) buf;
	(void) maxLength;
    return false;
}

void hal_nvram_setString(uint32_t addr, char* buf, int length)
{
	// TODO
	(void) addr;
	(void) buf;
	(void) length;
}

bool hal_nvram_getUInt32Value(uint32_t addr, uint32_t* value)
{
	// TODO
	(void) addr;
	(void) value;
    return false;
}

void hal_nvram_setUInt32Value(uint32_t addr, uint32_t value)
{
	// TODO
	(void) addr;
	(void) value;
}

bool hal_nvram_getUInt16Value(uint32_t addr, uint16_t* value)
{
	// TODO
	(void) addr;
	(void) value;
    return false;
}

void hal_nvram_setUInt16Value(uint32_t addr, uint16_t value)
{
	// TODO
	(void) addr;
	(void) value;
}

bool hal_nvram_getUInt8Value(uint32_t addr, uint8_t* value)
{
	// TODO
	(void) addr;
	(void) value;
    return false;
}

void hal_nvram_setUInt8Value(uint32_t addr, uint8_t value)
{
	// TODO
	(void) addr;
	(void) value;
}

bool hal_nvram_getfloat(uint32_t addr, float* value)
{
	// TODO
	(void) addr;
	(void) value;	
    return false;
}

void hal_nvram_setfloat(uint32_t addr, float value)
{
	// TODO
	(void) addr;
	(void) value;
}

