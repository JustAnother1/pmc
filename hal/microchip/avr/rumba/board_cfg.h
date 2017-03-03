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

// LED
#define DEBUG_LED_MASK                0x80
#define DEBUG_LED_DDR                 DDRB
#define DEBUG_LED_PIN                 PINB
#define DEBUG_LED_PORT                PORTB

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

// Timer

// Time Base (millisecond timer) -> Timer 1
#define CPU_MS_TIMER_TCCRA            TCCR1A
#define CPU_MS_TIMER_TCCRB            TCCR1B
#define CPU_MS_TIMER_TCCRC            TCCR1C
#define CPU_MS_TIMER_TCNT             TCNT1
#define CPU_MS_TIMER_OCRA             OCR1A
#define CPU_MS_TIMER_TIMSK            TIMSK1
#define CPU_MS_TIMER_RELOAD_VALUE     16000
#define CPU_MS_TIMER_COMPARE_A_ISR    TIMER1_COMPA_vect
#define CPU_MS_TIMER_OVERFLOW_ISR     TIMER1_OVF_vect


#endif /* HAL_MICROCHIP_AVR_RUMBA_BOARD_CFG_H_ */
