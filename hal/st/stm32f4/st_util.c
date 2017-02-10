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

uint_fast8_t copy_string(char * str, uint8_t *position)
{
    uint_fast8_t num = 0;
    char c = str[num];
    while(c !=0)
    {
        *position = c;
        num++;
        position++;
        c = str[num];
    }
    return num;
}

void print_gpio_configuration(GPIO_TypeDef * gpio)
{
    if(gpio == GPIOA) { debug_line("Port A:"); }
    if(gpio == GPIOB) { debug_line("Port B:"); }
    if(gpio == GPIOC) { debug_line("Port C:"); }
    if(gpio == GPIOD) { debug_line("Port D:"); }
    if(gpio == GPIOE) { debug_line("Port E:"); }
    if(gpio == GPIOF) { debug_line("Port F:"); }
    if(gpio == GPIOG) { debug_line("Port G:"); }
    if(gpio == GPIOH) { debug_line("Port H:"); }
    if(gpio == GPIOI) { debug_line("Port I:"); }

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

void print_gpio_pin_configuration(GPIO_TypeDef * PortRegisters, int idx)
{
    uint32_t val;
    if((0 > idx) || (15 < idx))
    {
        debug_line("Invalid Index !");
        return;
    }
    // Moder
    debug_line("GPIO->MODER   = 0x%08x", PortRegisters->MODER);
    val = (PortRegisters->MODER) >> (idx*2);
    val = val &3;
    switch(val)
    {
    case 0: debug_line("type          : Input Pin"); break;
    case 1: debug_line("type          : Output Pin"); break;
    case 2: debug_line("type          : Alternate Function Pin"); break;
    case 3: debug_line("type          : Analog Pin"); break;
    }
    // Typer
    debug_line("GPIO->OTYPER  = 0x%08x", PortRegisters->OTYPER);
    val = (PortRegisters->OTYPER) >> idx;
    val = val &1;
    switch(val)
    {
    case 0: debug_line("Output        : Push Pull"); break;
    case 1: debug_line("Output        : Open Drain"); break;
    }
    // Speedr
    debug_line("GPIO->OSPEEDR = 0x%08x", PortRegisters->OSPEEDR);
    val = (PortRegisters->OSPEEDR) >> (idx*2);
    val = val &3;
    switch(val)
    {
    case 0: debug_line("Output        : low speed"); break;
    case 1: debug_line("Output        : medium speed"); break;
    case 2: debug_line("Output        : fast speed"); break;
    case 3: debug_line("Output        : high speed"); break;
    }
    // pupdr
    debug_line("GPIO->PUPDR   = 0x%08x", PortRegisters->PUPDR);
    val = (PortRegisters->PUPDR) >> (idx*2);
    val = val &3;
    switch(val)
    {
    case 0: debug_line("pull up       : no");
            debug_line("pull down     : no"); break;
    case 1: debug_line("pull up       : yes");
            debug_line("pull down     : no"); break;
    case 2: debug_line("pull up       : no");
            debug_line("pull down     : yes"); break;
    case 3: debug_line("pull up       : ERROR: r e s e r v e d");
            debug_line("pull down     : ERROR: r e s e r v e d"); break;
    }
    // IDR
    debug_line("GPIO->IDR     = 0x%08x", PortRegisters->IDR);
    val = (PortRegisters->IDR) >> idx;
    val = val &1;
    switch(val)
    {
    case 0: debug_line("Input         : Pin is Low"); break;
    case 1: debug_line("Input         : Pin is High"); break;
    }
    // ODR
    debug_line("GPIO->ODR     = 0x%08x", PortRegisters->ODR);
    val = (PortRegisters->ODR) >> idx;
    val = val &1;
    switch(val)
    {
    case 0: debug_line("Output        : Pin should be Low"); break;
    case 1: debug_line("Output        : Pin should be High"); break;
    }
    debug_line("GPIO->AFR[0]  = 0x%08x", PortRegisters->AFR[0]);
    debug_line("GPIO->AFR[1]  = 0x%08x", PortRegisters->AFR[1]);
    if(idx < 8)
    {
        val = (PortRegisters->AFR[0]) >> (idx*4);
        val = val &15;
        switch(val)
        {
        case  0: debug_line("Alternate Function 0" ); break;
        case  1: debug_line("Alternate Function 1" ); break;
        case  2: debug_line("Alternate Function 2" ); break;
        case  3: debug_line("Alternate Function 3" ); break;
        case  4: debug_line("Alternate Function 4" ); break;
        case  5: debug_line("Alternate Function 5" ); break;
        case  6: debug_line("Alternate Function 6" ); break;
        case  7: debug_line("Alternate Function 7" ); break;
        case  8: debug_line("Alternate Function 8" ); break;
        case  9: debug_line("Alternate Function 9" ); break;
        case 10: debug_line("Alternate Function 10"); break;
        case 11: debug_line("Alternate Function 11"); break;
        case 12: debug_line("Alternate Function 12"); break;
        case 13: debug_line("Alternate Function 13"); break;
        case 14: debug_line("Alternate Function 14"); break;
        case 15: debug_line("Alternate Function 15"); break;
        }
    }
    else
    {
        idx = idx -8;
        val = (PortRegisters->AFR[1]) >> (idx*4);
        val = val &15;
        switch(val)
        {
        case  0: debug_line("Alternate Function 0" ); break;
        case  1: debug_line("Alternate Function 1" ); break;
        case  2: debug_line("Alternate Function 2" ); break;
        case  3: debug_line("Alternate Function 3" ); break;
        case  4: debug_line("Alternate Function 4" ); break;
        case  5: debug_line("Alternate Function 5" ); break;
        case  6: debug_line("Alternate Function 6" ); break;
        case  7: debug_line("Alternate Function 7" ); break;
        case  8: debug_line("Alternate Function 8" ); break;
        case  9: debug_line("Alternate Function 9" ); break;
        case 10: debug_line("Alternate Function 10"); break;
        case 11: debug_line("Alternate Function 11"); break;
        case 12: debug_line("Alternate Function 12"); break;
        case 13: debug_line("Alternate Function 13"); break;
        case 14: debug_line("Alternate Function 14"); break;
        case 15: debug_line("Alternate Function 15"); break;
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

