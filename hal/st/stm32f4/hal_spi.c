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

// 20 bit per stepper in Bytes
//(1 Stepper =  3 Bytes; ( 4 bits unused)
// 2 Stepper =  5 Bytes;
// 4 Stepper = 10 Bytes
// 6 Stepper = 15 Bytes
// 8 Stepper = 20 Bytes)
// #define SPI_BUFFER_LENGTH           20

// On the Wire : Most Significant Bit First (CR1)
// When a master is communicating with SPI slaves which need to be de-selected between
// transmissions, the NSS pin must be configured as GPIO or another GPIO must be used and
// toggled by software.- Note on Page 866 Reference Manual

static void start_spi_transaction(uint_fast8_t device,
                                  uint8_t *data_to_send,
                                  uint_fast8_t idx_of_first_byte,
                                  uint_fast8_t num_bytes_to_send,
                                  uint8_t *data_received);


typedef struct {
    // The index of the byte in the current buffer that will be send next,
    // will be decremented to 0 - end of message
    uint_fast8_t spi_offset;
    // pointer to Bytes to send
    uint8_t *spi_send_buffer;
    uint8_t *spi_receive_buffer;
    // SPI is idle
    bool spi_idle;
    SPI_TypeDef * bus;
}spi_device_typ;

static spi_device_typ devices[MAX_SPI + 1]; // +1 as MAX_SPI is the highest index into this array


void hal_spi_init(uint_fast8_t device)
{
    // initialize SPI Hardware
    devices[device].spi_idle = true;

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
        SPI_0_NSS_GPIO_PORT->AFR[0]  |=  SPI_0_NSS_GPIO_AFR_0_1;
        SPI_0_NSS_GPIO_PORT->AFR[0]  &= ~SPI_0_NSS_GPIO_AFR_0_0;
        SPI_0_NSS_GPIO_PORT->AFR[1]  |=  SPI_0_NSS_GPIO_AFR_1_1;
        SPI_0_NSS_GPIO_PORT->AFR[1]  &= ~SPI_0_NSS_GPIO_AFR_1_0;
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

        // configure SPI parameters
        // CPOL = 1 CPHA = 1
        SPI_0->CR1 = 0x0047;
        // No interrupts, TI Frame, no DMA  // TODO
        SPI_0->CR2 = 0x0014;
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
        SPI_1_NSS_GPIO_PORT->AFR[0]  |=  SPI_1_NSS_GPIO_AFR_0_1;
        SPI_1_NSS_GPIO_PORT->AFR[0]  &= ~SPI_1_NSS_GPIO_AFR_0_0;
        SPI_1_NSS_GPIO_PORT->AFR[1]  |=  SPI_1_NSS_GPIO_AFR_1_1;
        SPI_1_NSS_GPIO_PORT->AFR[1]  &= ~SPI_1_NSS_GPIO_AFR_1_0;
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

        // configure SPI parameters
        // CPOL = 1 CPHA = 1
        SPI_1->CR1 = 0x0047;
        // No interrupts, TI Frame, no DMA  // TODO
        SPI_1->CR2 = 0x0014;
        devices[device].bus = SPI_1;
        break;

    default:
        // specified an unavailable Interface
        hal_cpu_die();
        return;
    }


}

void hal_spi_do_transaction(uint_fast8_t device,
                            uint8_t *data_to_send,
                            uint_fast8_t idx_of_first_byte,
                            uint_fast8_t num_bytes_to_send,
                            uint8_t *data_received)
{
    start_spi_transaction(device,
                          data_to_send,
                          idx_of_first_byte,
                          num_bytes_to_send,
                          data_received);
    /*
    while(false == spi_idle)
    {
        ; // wait until we have the data back;
    }
    */
}

static void start_spi_transaction(uint_fast8_t device,
                                  uint8_t *data_to_send,
                                  uint_fast8_t idx_of_first_byte,
                                  uint_fast8_t num_bytes_to_send,
                                  uint8_t *data_received)
{
    devices[device].spi_send_buffer = data_to_send;
    devices[device].spi_receive_buffer = data_received;
    devices[device].spi_idle = false;
    devices[device].spi_offset = idx_of_first_byte;
    devices[device].bus->DR = devices[device].spi_send_buffer[idx_of_first_byte];
    //TODO
}

/*
INTERRUPT(spi_isr, INTERRUPT_SPI0)
{

    SPI0CN &= ~0x80; // reset Interrupt Flag
    if(0 == spi_offset)
    {
        spi_receive_buffer[0] = SPI0DAT;
        NSS = 1;
        spi_idle = TRUE;
        return;
    }
    else
    {
        spi_receive_buffer[spi_offset] = SPI0DAT;
        spi_offset--;
        SPI0DAT = spi_send_buffer[spi_offset];
        return;
    }

}
*/
