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

#include "util.h"
#include "hal_debug.h"

uint_fast8_t copy_string(char * str, uint8_t *position)
{
    uint_fast8_t num = 0;
    char c = str[num];
    while(c !=0)
    {
        *position = c;
        num++;
        c = str[num];
    }
    return num;
}

void print_gpio_configuration(GPIO_TypeDef * gpio)
{
    debug_line("GPIO->AFRL    = 0x%08x", gpio->AFR[0]);
    debug_line("GPIO->AFRH    = 0x%08x", gpio->AFR[1]);
    debug_line("GPIO->IDR     = 0x%08x", gpio->IDR);
    debug_line("GPIO->LCKR    = 0x%08x", gpio->LCKR);
    debug_line("GPIO->MODER   = 0x%08x", gpio->MODER);
    debug_line("GPIO->ODR     = 0x%08x", gpio->ODR);
    debug_line("GPIO->OSPEEDR = 0x%08x", gpio->OSPEEDR);
    debug_line("GPIO->OTYPER  = 0x%08x", gpio->OTYPER);
    debug_line("GPIO->PUPDR   = 0x%08x", gpio->PUPDR);
}
