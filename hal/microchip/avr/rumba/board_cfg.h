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

#ifndef HAL_MICROCHIP_AVR_RUMBA_BOARD_CFG_H_
#define HAL_MICROCHIP_AVR_RUMBA_BOARD_CFG_H_

// UART
#define GCODE_USART_NUMBER            0
#define GCODE_USART_UCSRA             UCSR0A
#define GCODE_USART_UCSRB             UCSR0B
#define GCODE_USART_UCSRC             UCSR0C
#define GCODE_USART_UDR               UDR0
#define GCODE_USART_U2X               true
#define GCODE_USART_UBRRH             UBRR0H
#define GCODE_USART_UBRRL             UBRR0L
// 115200 -> 16 U2X= true; 250k -> 3 U2X = false
#define GCODE_USART_BAUD_RATE_HIGH    0
#define GCODE_USART_BAUD_RATE_LOW     16
#define GCODE_RECEIVE_ISR             USART0_RX_vect
#define GCODE_TRANSMITT_COMPLETE_ISR  USART0_TX_vect
#define GCODE_DATA_REGISTER_EMPTY_ISR USART0_UDRE_vect

#define DEBUG_USART_NUMBER            3
#define DEBUG_USART_UCSRA             UCSR3A
#define DEBUG_USART_UCSRB             UCSR3B
#define DEBUG_USART_UCSRC             UCSR3C
#define DEBUG_USART_UDR               UDR3
#define DEBUG_USART_U2X               true
#define DEBUG_USART_UBRRH             UBRR3H
#define DEBUG_USART_UBRRL             UBRR3L
// 115200 -> 16 U2X= true; 250k -> 3 U2X = false
#define DEBUG_USART_BAUD_RATE_HIGH    0
#define DEBUG_USART_BAUD_RATE_LOW     16
#define DEBUG_RECEIVE_ISR             USART3_RX_vect
#define DEBUG_TRANSMITT_COMPLETE_ISR  USART3_TX_vect
#define DEBUG_DATA_REGISTER_EMPTY_ISR USART3_UDRE_vect

#endif /* HAL_MICROCHIP_AVR_RUMBA_BOARD_CFG_H_ */
