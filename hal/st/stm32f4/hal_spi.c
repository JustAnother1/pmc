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

#include "hal_spi.h"
#include "hal_cfg.h"
#include "hal_cpu.h"
#include <stdbool.h>
#include "spi.h"
#include "rcc.h"
#include "gpio.h"
#include "board_cfg.h"
#include "hal_debug.h"
#include "util.h"

// On the Wire : Most Significant Bit First (CR1)
// When a master is communicating with SPI slaves which need to be de-selected between
// transmissions, the NSS pin must be configured as GPIO or another GPIO must be used and
// toggled by software.- Note on Page 866 Reference Manual
// -> NSS is GPIO controlled by software

typedef struct {
    uint_fast8_t send_pos;
    uint_fast8_t rec_pos;
    uint_fast8_t length;
    // pointer to Bytes to send
    uint8_t *send_buffer;
    uint8_t *receive_buffer;
    // SPI is idle
    bool idle;
    SPI_TypeDef * bus;
}spi_device_typ;


static void start_spi_transaction(uint_fast8_t device,
                                  uint8_t *data_to_send,
                                  uint_fast8_t num_bytes_to_send,
                                  uint8_t *data_received);
static void slave_select_start(uint_fast8_t device);
static void slave_select_end(uint_fast8_t device);
static void device_IRQ_handler(uint_fast8_t device);

static volatile spi_device_typ devices[MAX_SPI];


void hal_spi_init(uint_fast8_t device)
{
    if(device < MAX_SPI)
    {
        // initialize SPI Hardware
        devices[device].idle = true;
        slave_select_end(device);

        switch(device)
        {
        // PINS: MISO, MOSI. NSS, SCK
        case 0 :
            // enable clock for GPIO Port
            RCC->AHB1ENR |= SPI_0_MISO_GPIO_PORT_RCC;
            RCC->AHB1ENR |= SPI_0_MOSI_GPIO_PORT_RCC;
            RCC->AHB1ENR |= SPI_0_NSS_GPIO_PORT_RCC;
            RCC->AHB1ENR |= SPI_0_SCK_GPIO_PORT_RCC;
            // enable clock for interface
            RCC->APB1ENR |= SPI_0_APB1ENR;
            RCC->APB2ENR |= SPI_0_APB2ENR;

            // configure SPI parameters
            // CPOL = 1 CPHA = 1
            SPI_0->CR1 = 0x037f;
            SPI_0->CR2 = 0x0040;

            // Enable Interrupt
            NVIC_SetPriority(SPI_0_IRQ_NUMBER, SPI_0_IRQ_PRIORITY);
            NVIC_EnableIRQ(SPI_0_IRQ_NUMBER);

            // configure Pins
            // MISO
            SPI_0_MISO_GPIO_PORT->MODER   |=  SPI_0_MISO_GPIO_MODER_1;
            SPI_0_MISO_GPIO_PORT->MODER   &= ~SPI_0_MISO_GPIO_MODER_0;
            SPI_0_MISO_GPIO_PORT->OTYPER  |=  SPI_0_MISO_GPIO_OTYPER_1;
            SPI_0_MISO_GPIO_PORT->OTYPER  &= ~SPI_0_MISO_GPIO_OTYPER_0;
            SPI_0_MISO_GPIO_PORT->OSPEEDR |=  SPI_0_MISO_GPIO_OSPEEDR_1;
            SPI_0_MISO_GPIO_PORT->OSPEEDR &= ~SPI_0_MISO_GPIO_OSPEEDR_0;
            SPI_0_MISO_GPIO_PORT->PUPDR   |=  SPI_0_MISO_GPIO_PUPD_1;
            SPI_0_MISO_GPIO_PORT->PUPDR   &= ~SPI_0_MISO_GPIO_PUPD_0;
            SPI_0_MISO_GPIO_PORT->AFR[0]  |=  SPI_0_MISO_GPIO_AFR_0_1;
            SPI_0_MISO_GPIO_PORT->AFR[0]  &= ~SPI_0_MISO_GPIO_AFR_0_0;
            SPI_0_MISO_GPIO_PORT->AFR[1]  |=  SPI_0_MISO_GPIO_AFR_1_1;
            SPI_0_MISO_GPIO_PORT->AFR[1]  &= ~SPI_0_MISO_GPIO_AFR_1_0;
            // MOSI
            SPI_0_MOSI_GPIO_PORT->MODER   |=  SPI_0_MOSI_GPIO_MODER_1;
            SPI_0_MOSI_GPIO_PORT->MODER   &= ~SPI_0_MOSI_GPIO_MODER_0;
            SPI_0_MOSI_GPIO_PORT->OTYPER  |=  SPI_0_MOSI_GPIO_OTYPER_1;
            SPI_0_MOSI_GPIO_PORT->OTYPER  &= ~SPI_0_MOSI_GPIO_OTYPER_0;
            SPI_0_MOSI_GPIO_PORT->OSPEEDR |=  SPI_0_MOSI_GPIO_OSPEEDR_1;
            SPI_0_MOSI_GPIO_PORT->OSPEEDR &= ~SPI_0_MOSI_GPIO_OSPEEDR_0;
            SPI_0_MOSI_GPIO_PORT->PUPDR   |=  SPI_0_MOSI_GPIO_PUPD_1;
            SPI_0_MOSI_GPIO_PORT->PUPDR   &= ~SPI_0_MOSI_GPIO_PUPD_0;
            SPI_0_MOSI_GPIO_PORT->AFR[0]  |=  SPI_0_MOSI_GPIO_AFR_0_1;
            SPI_0_MOSI_GPIO_PORT->AFR[0]  &= ~SPI_0_MOSI_GPIO_AFR_0_0;
            SPI_0_MOSI_GPIO_PORT->AFR[1]  |=  SPI_0_MOSI_GPIO_AFR_1_1;
            SPI_0_MOSI_GPIO_PORT->AFR[1]  &= ~SPI_0_MOSI_GPIO_AFR_1_0;
            // NSS
            SPI_0_NSS_GPIO_PORT->MODER   |=  SPI_0_NSS_GPIO_MODER_1;
            SPI_0_NSS_GPIO_PORT->MODER   &= ~SPI_0_NSS_GPIO_MODER_0;
            SPI_0_NSS_GPIO_PORT->OTYPER  |=  SPI_0_NSS_GPIO_OTYPER_1;
            SPI_0_NSS_GPIO_PORT->OTYPER  &= ~SPI_0_NSS_GPIO_OTYPER_0;
            SPI_0_NSS_GPIO_PORT->OSPEEDR |=  SPI_0_NSS_GPIO_OSPEEDR_1;
            SPI_0_NSS_GPIO_PORT->OSPEEDR &= ~SPI_0_NSS_GPIO_OSPEEDR_0;
            SPI_0_NSS_GPIO_PORT->PUPDR   |=  SPI_0_NSS_GPIO_PUPD_1;
            SPI_0_NSS_GPIO_PORT->PUPDR   &= ~SPI_0_NSS_GPIO_PUPD_0;
            // SCK
            SPI_0_SCK_GPIO_PORT->MODER   |=  SPI_0_SCK_GPIO_MODER_1;
            SPI_0_SCK_GPIO_PORT->MODER   &= ~SPI_0_SCK_GPIO_MODER_0;
            SPI_0_SCK_GPIO_PORT->OTYPER  |=  SPI_0_SCK_GPIO_OTYPER_1;
            SPI_0_SCK_GPIO_PORT->OTYPER  &= ~SPI_0_SCK_GPIO_OTYPER_0;
            SPI_0_SCK_GPIO_PORT->OSPEEDR |=  SPI_0_SCK_GPIO_OSPEEDR_1;
            SPI_0_SCK_GPIO_PORT->OSPEEDR &= ~SPI_0_SCK_GPIO_OSPEEDR_0;
            SPI_0_SCK_GPIO_PORT->PUPDR   |=  SPI_0_SCK_GPIO_PUPD_1;
            SPI_0_SCK_GPIO_PORT->PUPDR   &= ~SPI_0_SCK_GPIO_PUPD_0;
            SPI_0_SCK_GPIO_PORT->AFR[0]  |=  SPI_0_SCK_GPIO_AFR_0_1;
            SPI_0_SCK_GPIO_PORT->AFR[0]  &= ~SPI_0_SCK_GPIO_AFR_0_0;
            SPI_0_SCK_GPIO_PORT->AFR[1]  |=  SPI_0_SCK_GPIO_AFR_1_1;
            SPI_0_SCK_GPIO_PORT->AFR[1]  &= ~SPI_0_SCK_GPIO_AFR_1_0;

            devices[device].bus = SPI_0;
            break;

        case 1:
            // enable clock for GPIO Port
            RCC->AHB1ENR |= SPI_1_MISO_GPIO_PORT_RCC;
            RCC->AHB1ENR |= SPI_1_MOSI_GPIO_PORT_RCC;
            RCC->AHB1ENR |= SPI_1_NSS_GPIO_PORT_RCC;
            RCC->AHB1ENR |= SPI_1_SCK_GPIO_PORT_RCC;
            // enable clock for interface
            RCC->APB1ENR |= SPI_1_APB1ENR;
            RCC->APB2ENR |= SPI_1_APB2ENR;

            // configure SPI parameters
            // CPOL = 1 CPHA = 1
            SPI_1->CR1 = 0x037f;
            SPI_1->CR2 = 0x0040;

            // Enable Interrupt
            NVIC_SetPriority(SPI_1_IRQ_NUMBER, SPI_1_IRQ_PRIORITY);
            NVIC_EnableIRQ(SPI_1_IRQ_NUMBER);

            // configure Pins
            // MISO
            SPI_1_MISO_GPIO_PORT->MODER   |=  SPI_1_MISO_GPIO_MODER_1;
            SPI_1_MISO_GPIO_PORT->MODER   &= ~SPI_1_MISO_GPIO_MODER_0;
            SPI_1_MISO_GPIO_PORT->OTYPER  |=  SPI_1_MISO_GPIO_OTYPER_1;
            SPI_1_MISO_GPIO_PORT->OTYPER  &= ~SPI_1_MISO_GPIO_OTYPER_0;
            SPI_1_MISO_GPIO_PORT->OSPEEDR |=  SPI_1_MISO_GPIO_OSPEEDR_1;
            SPI_1_MISO_GPIO_PORT->OSPEEDR &= ~SPI_1_MISO_GPIO_OSPEEDR_0;
            SPI_1_MISO_GPIO_PORT->PUPDR   |=  SPI_1_MISO_GPIO_PUPD_1;
            SPI_1_MISO_GPIO_PORT->PUPDR   &= ~SPI_1_MISO_GPIO_PUPD_0;
            SPI_1_MISO_GPIO_PORT->AFR[0]  |=  SPI_1_MISO_GPIO_AFR_0_1;
            SPI_1_MISO_GPIO_PORT->AFR[0]  &= ~SPI_1_MISO_GPIO_AFR_0_0;
            SPI_1_MISO_GPIO_PORT->AFR[1]  |=  SPI_1_MISO_GPIO_AFR_1_1;
            SPI_1_MISO_GPIO_PORT->AFR[1]  &= ~SPI_1_MISO_GPIO_AFR_1_0;
            // MOSI
            SPI_1_MOSI_GPIO_PORT->MODER   |=  SPI_1_MOSI_GPIO_MODER_1;
            SPI_1_MOSI_GPIO_PORT->MODER   &= ~SPI_1_MOSI_GPIO_MODER_0;
            SPI_1_MOSI_GPIO_PORT->OTYPER  |=  SPI_1_MOSI_GPIO_OTYPER_1;
            SPI_1_MOSI_GPIO_PORT->OTYPER  &= ~SPI_1_MOSI_GPIO_OTYPER_0;
            SPI_1_MOSI_GPIO_PORT->OSPEEDR |=  SPI_1_MOSI_GPIO_OSPEEDR_1;
            SPI_1_MOSI_GPIO_PORT->OSPEEDR &= ~SPI_1_MOSI_GPIO_OSPEEDR_0;
            SPI_1_MOSI_GPIO_PORT->PUPDR   |=  SPI_1_MOSI_GPIO_PUPD_1;
            SPI_1_MOSI_GPIO_PORT->PUPDR   &= ~SPI_1_MOSI_GPIO_PUPD_0;
            SPI_1_MOSI_GPIO_PORT->AFR[0]  |=  SPI_1_MOSI_GPIO_AFR_0_1;
            SPI_1_MOSI_GPIO_PORT->AFR[0]  &= ~SPI_1_MOSI_GPIO_AFR_0_0;
            SPI_1_MOSI_GPIO_PORT->AFR[1]  |=  SPI_1_MOSI_GPIO_AFR_1_1;
            SPI_1_MOSI_GPIO_PORT->AFR[1]  &= ~SPI_1_MOSI_GPIO_AFR_1_0;
            // NSS
            SPI_1_NSS_GPIO_PORT->MODER   |=  SPI_1_NSS_GPIO_MODER_1;
            SPI_1_NSS_GPIO_PORT->MODER   &= ~SPI_1_NSS_GPIO_MODER_0;
            SPI_1_NSS_GPIO_PORT->OTYPER  |=  SPI_1_NSS_GPIO_OTYPER_1;
            SPI_1_NSS_GPIO_PORT->OTYPER  &= ~SPI_1_NSS_GPIO_OTYPER_0;
            SPI_1_NSS_GPIO_PORT->OSPEEDR |=  SPI_1_NSS_GPIO_OSPEEDR_1;
            SPI_1_NSS_GPIO_PORT->OSPEEDR &= ~SPI_1_NSS_GPIO_OSPEEDR_0;
            SPI_1_NSS_GPIO_PORT->PUPDR   |=  SPI_1_NSS_GPIO_PUPD_1;
            SPI_1_NSS_GPIO_PORT->PUPDR   &= ~SPI_1_NSS_GPIO_PUPD_0;
            // SCK
            SPI_1_SCK_GPIO_PORT->MODER   |=  SPI_1_SCK_GPIO_MODER_1;
            SPI_1_SCK_GPIO_PORT->MODER   &= ~SPI_1_SCK_GPIO_MODER_0;
            SPI_1_SCK_GPIO_PORT->OTYPER  |=  SPI_1_SCK_GPIO_OTYPER_1;
            SPI_1_SCK_GPIO_PORT->OTYPER  &= ~SPI_1_SCK_GPIO_OTYPER_0;
            SPI_1_SCK_GPIO_PORT->OSPEEDR |=  SPI_1_SCK_GPIO_OSPEEDR_1;
            SPI_1_SCK_GPIO_PORT->OSPEEDR &= ~SPI_1_SCK_GPIO_OSPEEDR_0;
            SPI_1_SCK_GPIO_PORT->PUPDR   |=  SPI_1_SCK_GPIO_PUPD_1;
            SPI_1_SCK_GPIO_PORT->PUPDR   &= ~SPI_1_SCK_GPIO_PUPD_0;
            SPI_1_SCK_GPIO_PORT->AFR[0]  |=  SPI_1_SCK_GPIO_AFR_0_1;
            SPI_1_SCK_GPIO_PORT->AFR[0]  &= ~SPI_1_SCK_GPIO_AFR_0_0;
            SPI_1_SCK_GPIO_PORT->AFR[1]  |=  SPI_1_SCK_GPIO_AFR_1_1;
            SPI_1_SCK_GPIO_PORT->AFR[1]  &= ~SPI_1_SCK_GPIO_AFR_1_0;

            devices[device].bus = SPI_1;
            break;

        default:
            // specified an unavailable Interface
            hal_cpu_die();
            return;
        }
    }
    // else invalid Interface Specified
}

void hal_spi_print_configuration(uint_fast8_t device)
{
    if(device < MAX_SPI)
    {
        debug_line("Configuration of SPI_%d :", device);
        // Clock
        debug_line("RCC->AHB1ENR  = 0x%08x", RCC->AHB1ENR);
        debug_line("RCC->APB1ENR  = 0x%08x", RCC->APB1ENR);
        debug_line("RCC->APB2ENR  = 0x%08x", RCC->APB2ENR);
        // Uart
        debug_line("SPI->CR1      = 0x%08x", devices[device].bus->CR1);
        debug_line("SPI->CR2      = 0x%08x", devices[device].bus->CR2);
        // GPIO
        switch(device)
        {
        case 0 :
            debug_line("MISO Pin:");
            print_gpio_configuration(SPI_0_MISO_GPIO_PORT);
            debug_line("MOSI Pin:");
            print_gpio_configuration(SPI_0_MOSI_GPIO_PORT);
            debug_line("SCK Pin:");
            print_gpio_configuration(SPI_0_NSS_GPIO_PORT);
            debug_line("NSS Pin:");
            print_gpio_configuration(SPI_0_SCK_GPIO_PORT);
            break;

        case 1 :
            debug_line("MISO Pin:");
            print_gpio_configuration(SPI_1_MISO_GPIO_PORT);
            debug_line("MOSI Pin:");
            print_gpio_configuration(SPI_1_MOSI_GPIO_PORT);
            debug_line("SCK Pin:");
            print_gpio_configuration(SPI_1_NSS_GPIO_PORT);
            debug_line("NSS Pin:");
            print_gpio_configuration(SPI_1_SCK_GPIO_PORT);
            break;

        default:
            break;
        }
    }
    // else invalid Interface Specified
}

void SPI_0_IRQ_HANDLER(void)
{
    device_IRQ_handler(0);
}

void SPI_1_IRQ_HANDLER(void)
{
    device_IRQ_handler(1);
}

static void device_IRQ_handler(uint_fast8_t device)
{
    if(SPI_SR_RXNE == (SPI_SR_RXNE & devices[device].bus->SR))
    {
        // we received a byte
        devices[device].receive_buffer[devices[device].rec_pos] = (uint8_t)devices[device].bus->DR;
        devices[device].rec_pos++;
        if(devices[device].length == devices[device].rec_pos)
        {
            // we received the last byte
            slave_select_end(device);
            devices[device].idle = true;
        }
    }
    if(SPI_SR_TXE == (SPI_SR_TXE & devices[device].bus->SR))
    {
        // send the next byte
        if(devices[device].length > devices[device].send_pos)
        {
            devices[device].bus->DR = devices[device].send_buffer[devices[device].send_pos];
            devices[device].send_pos++;
        }
        if(devices[device].length == devices[device].send_pos)
        {
            // we send the last byte
            devices[device].bus->CR2 &= ~SPI_CR2_TXEIE;
        }
        devices[device].bus->SR &= ~SPI_SR_TXE;
    }
}

void hal_spi_do_transaction(uint_fast8_t device,
                            uint8_t *data_to_send,
                            uint_fast8_t num_bytes_to_send,
                            uint8_t *data_received)
{
    if(device < MAX_SPI)
    {
        while(false == devices[device].idle)
        {
            ; // wait until we can send the data out
        }
        start_spi_transaction(device,
                              data_to_send,
                              num_bytes_to_send,
                              data_received);

        while(false == devices[device].idle)
        {
            ; // wait until we have the data back
        }
    }
    // else invalid Interface Specified
}

static void slave_select_start(uint_fast8_t device)
{
    // NSS -> LOW
    switch(device)
    {
    case 0 : SPI_0_NSS_GPIO_PORT->BSRR_RESET = SPI_0_NSS_GPIO_BSRR; break;
    case 1 : SPI_1_NSS_GPIO_PORT->BSRR_RESET = SPI_1_NSS_GPIO_BSRR; break;
    default: break;
    }
}

static void slave_select_end(uint_fast8_t device)
{
    // NSS -> HIGH
    switch(device)
    {
    case 0 : SPI_0_NSS_GPIO_PORT->BSRR_SET = SPI_0_NSS_GPIO_BSRR; break;
    case 1 : SPI_1_NSS_GPIO_PORT->BSRR_SET = SPI_1_NSS_GPIO_BSRR; break;
    default: break;
    }
}

static void start_spi_transaction(uint_fast8_t device,
                                  uint8_t *data_to_send,
                                  uint_fast8_t num_bytes_to_send,
                                  uint8_t *data_received)
{
    slave_select_start(device);
    devices[device].send_buffer = data_to_send;
    devices[device].receive_buffer = data_received;
    devices[device].idle = false;
    devices[device].send_pos = 1;
    devices[device].rec_pos = 0;
    devices[device].length = num_bytes_to_send;
    devices[device].bus->DR = devices[device].send_buffer[0];
    devices[device].bus->CR2 |= SPI_CR2_TXEIE;
}
