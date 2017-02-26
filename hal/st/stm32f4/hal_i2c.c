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
#include "hal_cpu.h"
#include "hal_debug.h"
#include "hal_led.h"
#include "hal_time.h"
#include "st_gpio.h"
#include "st_rcc.h"
#include "st_i2c.h"
#include "st_util.h"

#define TIMEOUT_MS   500

enum i2c_state  {idle, start_send, device_address_send, byte_send,
                 repeated_start_send, device_read_address_send, read_a_byte};

typedef struct{
    bool read;
    uint_fast8_t i2cAddress;
    uint_fast8_t dataAddress;
    uint8_t*     data;
    uint_fast8_t num_bytes;
    uint_fast8_t send_bytes;
} transfer_Definition;

void I2C1_EV_IRQHandler(void) __attribute__ ((interrupt ("IRQ")));
void I2C1_ER_IRQHandler(void) __attribute__ ((interrupt ("IRQ")));

static bool transaction_successfull;
static enum i2c_state cur_state;
static transfer_Definition cur_transfer;
static bool initialized = false;

void hal_init_i2c(void)
{
    if(true == initialized)
    {
        // initialize only once !
        return;
    }
    initialized = true;
    transaction_successfull = false;  // not yet
    cur_state = idle;// no activities yet, the bus is ours!

    RCC->AHB1ENR |= I2C_0_SCL_GPIO_PORT_RCC;
    RCC->AHB1ENR |= I2C_0_SDA_GPIO_PORT_RCC;
    RCC->APB1ENR |= I2C_0_APB1ENR;

    // Sm = 100kHz, Fm = 400kHz
    // Peripheral Input Clock for Fast Mode must be at least 4MHz (CR2)

    I2C_0->CR1 = 0;
    I2C_0->CR2 = I2C_CR2_ITBUFEN | I2C_CR2_ITEVTEN | I2C_CR2_ITERREN | I2C_0_APB1_CLOCK;
    I2C_0->CCR = I2C_CCR_FS | 35; // 42MHz /( 400 kHz * 3)  -> 35
    I2C_0->TRISE = 14; // (0..63) 400kHz -> 300ms -> 14?
    I2C_0->FLTR = 0;// Filter not available on 405 -> not used

    I2C_0->CR1 = I2C_CR1_PE;

    // Enable Interrupt
    NVIC_SetPriority(I2C_0_ERROR_IRQ_NUMBER, I2C_0_ERROR_IRQ_PRIORITY);
    NVIC_EnableIRQ(I2C_0_ERROR_IRQ_NUMBER);

    NVIC_SetPriority(I2C_0_EVENT_IRQ_NUMBER, I2C_0_EVENT_IRQ_PRIORITY);
    NVIC_EnableIRQ(I2C_0_EVENT_IRQ_NUMBER);

    // SCL
    I2C_0_SCL_GPIO_PORT->MODER   |=  I2C_0_SCL_GPIO_MODER_1;
    I2C_0_SCL_GPIO_PORT->MODER   &= ~I2C_0_SCL_GPIO_MODER_0;
    I2C_0_SCL_GPIO_PORT->OTYPER  |=  I2C_0_SCL_GPIO_OTYPER_1;
    I2C_0_SCL_GPIO_PORT->OTYPER  &= ~I2C_0_SCL_GPIO_OTYPER_0;
    I2C_0_SCL_GPIO_PORT->OSPEEDR |=  I2C_0_SCL_GPIO_OSPEEDR_1;
    I2C_0_SCL_GPIO_PORT->OSPEEDR &= ~I2C_0_SCL_GPIO_OSPEEDR_0;
    I2C_0_SCL_GPIO_PORT->PUPDR   |=  I2C_0_SCL_GPIO_PUPD_1;
    I2C_0_SCL_GPIO_PORT->PUPDR   &= ~I2C_0_SCL_GPIO_PUPD_0;
    I2C_0_SCL_GPIO_PORT->AFR[0]  |=  I2C_0_SCL_GPIO_AFR_0_1;
    I2C_0_SCL_GPIO_PORT->AFR[0]  &= ~I2C_0_SCL_GPIO_AFR_0_0;
    I2C_0_SCL_GPIO_PORT->AFR[1]  |=  I2C_0_SCL_GPIO_AFR_1_1;
    I2C_0_SCL_GPIO_PORT->AFR[1]  &= ~I2C_0_SCL_GPIO_AFR_1_0;
    // SDA
    I2C_0_SDA_GPIO_PORT->MODER   |=  I2C_0_SDA_GPIO_MODER_1;
    I2C_0_SDA_GPIO_PORT->MODER   &= ~I2C_0_SDA_GPIO_MODER_0;
    I2C_0_SDA_GPIO_PORT->OTYPER  |=  I2C_0_SDA_GPIO_OTYPER_1;
    I2C_0_SDA_GPIO_PORT->OTYPER  &= ~I2C_0_SDA_GPIO_OTYPER_0;
    I2C_0_SDA_GPIO_PORT->OSPEEDR |=  I2C_0_SDA_GPIO_OSPEEDR_1;
    I2C_0_SDA_GPIO_PORT->OSPEEDR &= ~I2C_0_SDA_GPIO_OSPEEDR_0;
    I2C_0_SDA_GPIO_PORT->PUPDR   |=  I2C_0_SDA_GPIO_PUPD_1;
    I2C_0_SDA_GPIO_PORT->PUPDR   &= ~I2C_0_SDA_GPIO_PUPD_0;
    I2C_0_SDA_GPIO_PORT->AFR[0]  |=  I2C_0_SDA_GPIO_AFR_0_1;
    I2C_0_SDA_GPIO_PORT->AFR[0]  &= ~I2C_0_SDA_GPIO_AFR_0_0;
    I2C_0_SDA_GPIO_PORT->AFR[1]  |=  I2C_0_SDA_GPIO_AFR_1_1;
    I2C_0_SDA_GPIO_PORT->AFR[1]  &= ~I2C_0_SDA_GPIO_AFR_1_0;
}

#ifdef DEBUG_ACTIVE
void hal_print_i2c_configuration(void)
{
    debug_line(STR("Configuration of I2C :"));
    // Clock
    debug_line(STR("RCC->AHB1ENR  = 0x%08x"), RCC->AHB1ENR);
    debug_line(STR("RCC->APB1ENR  = 0x%08x"), RCC->APB1ENR);
    debug_line(STR("RCC->APB2ENR  = 0x%08x"), RCC->APB2ENR);
    // I2C
    debug_line(STR("I2C_0->CR1    = 0x%08x"), I2C_0->CR1);
    debug_line(STR("I2C_0->CR2    = 0x%08x"), I2C_0->CR2);
    debug_line(STR("I2C_0->OAR1   = 0x%08x"), I2C_0->OAR1);
    debug_line(STR("I2C_0->OAR2   = 0x%08x"), I2C_0->OAR2);
    debug_line(STR("I2C_0->DR     = 0x%08x"), I2C_0->DR);
    debug_line(STR("I2C_0->SR1    = 0x%08x"), I2C_0->SR1);
    debug_line(STR("I2C_0->SR2    = 0x%08x"), I2C_0->SR2);
    debug_line(STR("I2C_0->CCR    = 0x%08x"), I2C_0->CCR);
    debug_line(STR("I2C_0->TRISE  = 0x%08x"), I2C_0->TRISE);
    debug_line(STR("I2C_0->FLTR   = 0x%08x"), I2C_0->FLTR);
    // GPIO
    debug_line(STR("SCL Pin:"));
    print_gpio_pin_configuration(I2C_0_SCL_GPIO_PORT, I2C_0_SCL_GPIO_PIN);
    debug_line(STR("SDA Pin:"));
    print_gpio_pin_configuration(I2C_0_SCL_GPIO_PORT, I2C_0_SDA_GPIO_PIN);
}
#endif

// Interrupt Handlers
void I2C1_EV_IRQHandler(void)
{
    hal_set_isr1_led(true);
    switch(cur_state)
    {
    case idle:
        // debug_line(STR("ERROR: Interrupt in Idle! (SR1: 0x%08x SR2: 0x%08x)"), I2C_0->SR1, I2C_0->SR2);
        I2C_0->SR1 = 0;
        I2C_0->CR1 = I2C_CR1_STOP;
        break;

    case start_send:
        // MSL Bit = 1 and SB bit = 1 -> just send a start as beginning of transaction.
        // -> send Slave Device Address now. (Read SR1 then write DR)
        if(I2C_SR1_SB == (I2C_SR1_SB & I2C_0->SR1))
        {
            I2C_0->DR = cur_transfer.i2cAddress;
            cur_state = device_address_send;
        }
        else
        {
            debug_line(STR("ERROR: No start send ! (SR1: 0x%08x SR2: 0x%08x)"), I2C_0->SR1, I2C_0->SR2);
            cur_state = idle;
            I2C_0->CR1 = I2C_CR1_STOP;
        }
        break;

    case device_address_send:
        // ADDR Bit = 1; Read SR1 then read SR2
        // TRA bit decides if read oder write
        if(I2C_SR1_ADDR == (I2C_SR1_ADDR & I2C_0->SR1))
        {
            if(I2C_SR2_MSL != (I2C_SR2_MSL & I2C_0->SR2))
            {
                debug_line(STR("ERROR(da): We are not master !"));
                cur_state = idle;
                I2C_0->CR1 = I2C_CR1_STOP;
            }
            else
            {
                I2C_0->DR = cur_transfer.dataAddress;
                cur_state = byte_send;
            }
        }
        else
        {
            debug_line(STR("ERROR: No address send ! (SR1: 0x%08x SR2: 0x%08x)"), I2C_0->SR1, I2C_0->SR2);
            cur_state = idle;
            I2C_0->CR1 = I2C_CR1_STOP;
        }
        break;

    case byte_send:
        // TxE
        // debug_line(STR("Info: (SR1: 0x%08x SR2: 0x%08x)"), I2C_0->SR1, I2C_0->SR2);
        if(I2C_SR1_TXE == (I2C_SR1_TXE & I2C_0->SR1))
        {
            if(true == cur_transfer.read)
            {
                // send repeated Start
                I2C_0->CR1 |= I2C_CR1_START;
                cur_state = repeated_start_send;
            }
            else
            {
                if(cur_transfer.send_bytes == cur_transfer.num_bytes)
                {
                    // send stop
                    transaction_successfull = true;
                    I2C_0->CR1 |= I2C_CR1_STOP;
                    cur_state = idle;
                }
                else
                {
                    // send next byte
                    I2C_0->DR = cur_transfer.data[cur_transfer.send_bytes];
                    cur_transfer.send_bytes ++;
                    cur_state = byte_send;
                }
            }
        }
        else
        {
            debug_line(STR("ERROR: byte send failed ! (SR1: 0x%08x SR2: 0x%08x)"), I2C_0->SR1, I2C_0->SR2);
            cur_state = idle;
            I2C_0->CR1 = I2C_CR1_STOP;
        }
        break;

    case repeated_start_send:
        if(I2C_SR1_SB == (I2C_SR1_SB & I2C_0->SR1))
        {
            I2C_0->DR = cur_transfer.i2cAddress | 1;
            cur_state = device_read_address_send;
        }
        else
        {
            // TODO check that this does not happen too often ?
            /* TxE interrupts will happen before the Start is finished,..
            debug_line(STR("ERROR: No repeated start send ! (SR1: 0x%08x SR2: 0x%08x)"), I2C_0->SR1, I2C_0->SR2);
            cur_state = idle;
            I2C_0->CR1 = I2C_CR1_STOP;
            */
        }
        break;

    case device_read_address_send:
        if(I2C_SR1_ADDR == (I2C_SR1_ADDR & I2C_0->SR1))
        {
            if(I2C_SR2_MSL != (I2C_SR2_MSL & I2C_0->SR2)) // need to read SR2
            {
                debug_line(STR("ERROR(dra): We are not master !"));
                cur_state = idle;
                I2C_0->CR1 = I2C_CR1_STOP;
            }
            else
            {
                I2C_0->CR1 |= I2C_CR1_ACK;
                cur_state = read_a_byte;
            }
        }
        else
        {
            debug_line(STR("ERROR: No address send ! (SR1: 0x%08x SR2: 0x%08x)"), I2C_0->SR1, I2C_0->SR2);
            cur_state = idle;
            I2C_0->CR1 = I2C_CR1_STOP;
        }
        break;

    case read_a_byte:
        // read next byte
        cur_transfer.data[cur_transfer.send_bytes] = (uint8_t)(0xff & I2C_0->DR);
        cur_transfer.send_bytes ++;
        if(cur_transfer.send_bytes == cur_transfer.num_bytes - 1)
        {
            I2C_0->CR1 = (I2C_0->CR1 & ~I2C_CR1_ACK);
        }
        if(cur_transfer.send_bytes == cur_transfer.num_bytes)
        {
            // send stop
            transaction_successfull = true;
            I2C_0->CR1 |= I2C_CR1_STOP;
            cur_state = idle;
        }
        // else wait for next byte
        break;
    }
    hal_set_isr1_led(false);
}

void I2C1_ER_IRQHandler(void)
{
    hal_set_isr1_led(true);
    if(I2C_SR1_BERR == (I2C_SR1_BERR & I2C_0->SR1))
    {
        cur_state = idle;
        debug_line(STR("ERROR IRQ: Bus Error !"));
        I2C_0->SR1 = I2C_0->SR1 &~I2C_SR1_BERR;
        I2C_0->CR1 = 0;
    }
    if(I2C_SR1_ARLO == (I2C_SR1_ARLO & I2C_0->SR1))
    {
        cur_state = idle;
        debug_line(STR("ERROR IRQ: Arbitration lost !"));
        I2C_0->SR1 = I2C_0->SR1 &~I2C_SR1_ARLO;
        I2C_0->CR1 = 0;
    }
    if(I2C_SR1_AF == (I2C_SR1_AF & I2C_0->SR1))
    {
        cur_state = idle;
        debug_line(STR("ERROR IRQ: Acknowledge failure !"));
        I2C_0->SR1 = I2C_0->SR1 &~I2C_SR1_AF;
        I2C_0->CR1 = 0;
    }
    if(I2C_SR1_OVR == (I2C_SR1_OVR & I2C_0->SR1))
    {
        cur_state = idle;
        debug_line(STR("ERROR IRQ: Overrun / Underrun !"));
        I2C_0->SR1 = I2C_0->SR1 &~I2C_SR1_OVR;
        I2C_0->CR1 = 0;
    }
    if(I2C_SR1_PECERR == (I2C_SR1_PECERR & I2C_0->SR1))
    {
        cur_state = idle;
        debug_line(STR("ERROR IRQ: PEC error in Reception !"));
        I2C_0->SR1 = I2C_0->SR1 &~I2C_SR1_PECERR;
        I2C_0->CR1 = 0;
    }
    if(I2C_SR1_TIMEOUT == (I2C_SR1_TIMEOUT & I2C_0->SR1))
    {
        cur_state = idle;
        debug_line(STR("ERROR IRQ: Timeout / Tlow !"));
        I2C_0->SR1 = I2C_0->SR1 &~I2C_SR1_TIMEOUT;
        I2C_0->CR1 = 0;
    }
    if(I2C_SR1_SMBALERT == (I2C_SR1_SMBALERT & I2C_0->SR1))
    {
        cur_state = idle;
        debug_line(STR("ERROR IRQ: SMB Alert !"));
        I2C_0->SR1 = I2C_0->SR1 &~I2C_SR1_SMBALERT;
        I2C_0->CR1 = 0;
    }
    hal_set_isr1_led(false);
}

bool hal_do_i2c_transaction(bool read,
                            uint_fast8_t i2cAddress,
                            uint_fast8_t dataAddress,
                            uint8_t*     data,
                            uint_fast8_t num_bytes)
{
    uint32_t curTime = hal_cpu_get_ms_tick();
    uint32_t endTime = curTime + TIMEOUT_MS;
    while((cur_state != idle) && (endTime > curTime))
    {
        curTime = hal_cpu_get_ms_tick(); // wait until we can send the data out
    }
    if(endTime == curTime)
    {
        debug_line(STR("I2C not Idle !"));
        return false;
    }

    hal_start_i2c_transaction(read, i2cAddress, dataAddress, data, num_bytes);

    curTime = hal_cpu_get_ms_tick();
    endTime = curTime + TIMEOUT_MS;
    while((cur_state != idle) && (endTime > curTime))
    {
        curTime = hal_cpu_get_ms_tick(); // wait until we have the data back
    }
    if(endTime == curTime)
    {
        debug_line(STR("I2C Transaction never finished!"));
        cur_state = idle;
        return false;
    }
    return transaction_successfull;
}

void hal_start_i2c_transaction(bool read,
                               uint_fast8_t i2cAddress,
                               uint_fast8_t dataAddress,
                               uint8_t*     data,
                               uint_fast8_t num_bytes )
{
    if(idle != cur_state)
    {
        return;
    }
    I2C_0->CR1 = I2C_CR1_PE;
    cur_transfer.i2cAddress = i2cAddress;
    I2C_0->CR1 |= I2C_CR1_START;
    cur_transfer.read = read;
    cur_transfer.dataAddress = dataAddress;
    cur_transfer.data = data;
    cur_transfer.num_bytes = num_bytes;
    cur_transfer.send_bytes = 0; // no bytes send out yet
    cur_state = start_send;

}

bool hal_i2c_is_idle(void)
{
    if(idle == cur_state)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void hal_i2c_reset_transaction(void)
{
    cur_state = idle;

}

