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

#include <st_util.h>
#include "hal_debug.h"

void print_gpio_configuration(GPIO_TypeDef * gpio)
{
    if(gpio == GPIOA) { debug_line(STR("Port A:")); }
    if(gpio == GPIOB) { debug_line(STR("Port B:")); }
    if(gpio == GPIOC) { debug_line(STR("Port C:")); }
    if(gpio == GPIOD) { debug_line(STR("Port D:")); }
    if(gpio == GPIOE) { debug_line(STR("Port E:")); }
    if(gpio == GPIOF) { debug_line(STR("Port F:")); }
    if(gpio == GPIOG) { debug_line(STR("Port G:")); }
    if(gpio == GPIOH) { debug_line(STR("Port H:")); }
    if(gpio == GPIOI) { debug_line(STR("Port I:")); }

    debug_line(STR("GPIO->AFRL    = 0x%08x"), gpio->AFR[0]);
    debug_line(STR("GPIO->AFRH    = 0x%08x"), gpio->AFR[1]);
    debug_line(STR("GPIO->IDR     = 0x%08x"), gpio->IDR);
    debug_line(STR("GPIO->LCKR    = 0x%08x"), gpio->LCKR);
    debug_line(STR("GPIO->MODER   = 0x%08x"), gpio->MODER);
    debug_line(STR("GPIO->ODR     = 0x%08x"), gpio->ODR);
    debug_line(STR("GPIO->OSPEEDR = 0x%08x"), gpio->OSPEEDR);
    debug_line(STR("GPIO->OTYPER  = 0x%08x"), gpio->OTYPER);
    debug_line(STR("GPIO->PUPDR   = 0x%08x"), gpio->PUPDR);
}

void print_gpio_pin_configuration(GPIO_TypeDef * PortRegisters, int idx)
{
    uint32_t val;
    if((0 > idx) || (15 < idx))
    {
        debug_line(STR("Invalid Index !"));
        return;
    }
    // Moder
    debug_line(STR("GPIO->MODER   = 0x%08x"), PortRegisters->MODER);
    val = (PortRegisters->MODER) >> (idx*2);
    val = val &3;
    switch(val)
    {
    case 0: debug_line(STR("type          : Input Pin")); break;
    case 1: debug_line(STR("type          : Output Pin")); break;
    case 2: debug_line(STR("type          : Alternate Function Pin")); break;
    case 3: debug_line(STR("type          : Analog Pin")); break;
    }
    // Typer
    debug_line(STR("GPIO->OTYPER  = 0x%08x"), PortRegisters->OTYPER);
    val = (PortRegisters->OTYPER) >> idx;
    val = val &1;
    switch(val)
    {
    case 0: debug_line(STR("Output        : Push Pull")); break;
    case 1: debug_line(STR("Output        : Open Drain")); break;
    }
    // Speedr
    debug_line(STR("GPIO->OSPEEDR = 0x%08x"), PortRegisters->OSPEEDR);
    val = (PortRegisters->OSPEEDR) >> (idx*2);
    val = val &3;
    switch(val)
    {
    case 0: debug_line(STR("Output        : low speed")); break;
    case 1: debug_line(STR("Output        : medium speed")); break;
    case 2: debug_line(STR("Output        : fast speed")); break;
    case 3: debug_line(STR("Output        : high speed")); break;
    }
    // pupdr
    debug_line(STR("GPIO->PUPDR   = 0x%08x"), PortRegisters->PUPDR);
    val = (PortRegisters->PUPDR) >> (idx*2);
    val = val &3;
    switch(val)
    {
    case 0: debug_line(STR("pull up       : no"));
            debug_line(STR("pull down     : no")); break;
    case 1: debug_line(STR("pull up       : yes"));
            debug_line(STR("pull down     : no")); break;
    case 2: debug_line(STR("pull up       : no"));
            debug_line(STR("pull down     : yes")); break;
    case 3: debug_line(STR("pull up       : ERROR: r e s e r v e d"));
            debug_line(STR("pull down     : ERROR: r e s e r v e d")); break;
    }
    // IDR
    debug_line(STR("GPIO->IDR     = 0x%08x"), PortRegisters->IDR);
    val = (PortRegisters->IDR) >> idx;
    val = val &1;
    switch(val)
    {
    case 0: debug_line(STR("Input         : Pin is Low")); break;
    case 1: debug_line(STR("Input         : Pin is High")); break;
    }
    // ODR
    debug_line(STR("GPIO->ODR     = 0x%08x"), PortRegisters->ODR);
    val = (PortRegisters->ODR) >> idx;
    val = val &1;
    switch(val)
    {
    case 0: debug_line(STR("Output        : Pin should be Low")); break;
    case 1: debug_line(STR("Output        : Pin should be High")); break;
    }
    debug_line(STR("GPIO->AFR[0]  = 0x%08x"), PortRegisters->AFR[0]);
    debug_line(STR("GPIO->AFR[1]  = 0x%08x"), PortRegisters->AFR[1]);
    if(idx < 8)
    {
        val = (PortRegisters->AFR[0]) >> (idx*4);
        val = val &15;
        switch(val)
        {
        case  0: debug_line(STR("Alternate Function 0" )); break;
        case  1: debug_line(STR("Alternate Function 1" )); break;
        case  2: debug_line(STR("Alternate Function 2" )); break;
        case  3: debug_line(STR("Alternate Function 3" )); break;
        case  4: debug_line(STR("Alternate Function 4" )); break;
        case  5: debug_line(STR("Alternate Function 5" )); break;
        case  6: debug_line(STR("Alternate Function 6" )); break;
        case  7: debug_line(STR("Alternate Function 7" )); break;
        case  8: debug_line(STR("Alternate Function 8" )); break;
        case  9: debug_line(STR("Alternate Function 9" )); break;
        case 10: debug_line(STR("Alternate Function 10")); break;
        case 11: debug_line(STR("Alternate Function 11")); break;
        case 12: debug_line(STR("Alternate Function 12")); break;
        case 13: debug_line(STR("Alternate Function 13")); break;
        case 14: debug_line(STR("Alternate Function 14")); break;
        case 15: debug_line(STR("Alternate Function 15")); break;
        }
    }
    else
    {
        idx = idx -8;
        val = (PortRegisters->AFR[1]) >> (idx*4);
        val = val &15;
        switch(val)
        {
        case  0: debug_line(STR("Alternate Function 0" )); break;
        case  1: debug_line(STR("Alternate Function 1" )); break;
        case  2: debug_line(STR("Alternate Function 2" )); break;
        case  3: debug_line(STR("Alternate Function 3" )); break;
        case  4: debug_line(STR("Alternate Function 4" )); break;
        case  5: debug_line(STR("Alternate Function 5" )); break;
        case  6: debug_line(STR("Alternate Function 6" )); break;
        case  7: debug_line(STR("Alternate Function 7" )); break;
        case  8: debug_line(STR("Alternate Function 8" )); break;
        case  9: debug_line(STR("Alternate Function 9" )); break;
        case 10: debug_line(STR("Alternate Function 10")); break;
        case 11: debug_line(STR("Alternate Function 11")); break;
        case 12: debug_line(STR("Alternate Function 12")); break;
        case 13: debug_line(STR("Alternate Function 13")); break;
        case 14: debug_line(STR("Alternate Function 14")); break;
        case 15: debug_line(STR("Alternate Function 15")); break;
        }
    }
}


void uDelay(const uint32_t usec)
{
  uint32_t count = 0;
  const uint32_t utime = (40 * usec / 7);
  do
  {
    if ( ++count > utime )
    {
      return ;
    }
  }
  while(1);
}

void mDelay(const uint32_t msec)
{
  uDelay(msec * 1000);
}

