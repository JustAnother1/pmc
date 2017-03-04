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

#ifndef HAL_MICROCHIP_AVR_ULTIMAKER_ORIGINAL_BOARD_CFG_H_
#define HAL_MICROCHIP_AVR_ULTIMAKER_ORIGINAL_BOARD_CFG_H_

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
#define CPU_MS_TIMER_RELOAD_VALUE     (F_CPU/1000)
#define CPU_MS_TIMER_COMPARE_A_ISR    TIMER1_COMPA_vect
#define CPU_MS_TIMER_OVERFLOW_ISR     TIMER1_OVF_vect

// PWM
#define PWM_NUM_PINS                  3
#define PWM_NUM_PORTS                 2
#define PWM_PORT_1                    PORTE
#define PWM_DD_1                      DDRE
#define PWM_PORT_1_MASK               0x30; // E4; E5
#define PWM_PORT_2                    PORTG
#define PWM_DD_2                      DDRG
#define PWM_PORT_2_MASK               0x20; // G5

#define PWM_0_NAME                    PSTR("Heated Bed")
#define PWM_0_OCR                     OCR0B
#define PWM_0_TCNT                    TCNT0
#define PWM_0_TIMSK                   TIMSK0
#define PWM_0_TCCRA                   TCCR0A
#define PWM_0_TCCRB                   TCCR0B

#define PWM_1_NAME                    PSTR("Heater 1")
#define PWM_1_OCR                     OCR3C
#define PWM_1_TCNT                    TCNT3
#define PWM_1_TIMSK                   TIMSK3
#define PWM_1_TCCRA                   TCCR3A
#define PWM_1_TCCRB                   TCCR3B

#define PWM_2_NAME                    PSTR("Heater 2")
#define PWM_2_OCR                     OCR3B
#define PWM_2_TCNT                    TCNT3
#define PWM_2_TIMSK                   TIMSK3
#define PWM_2_TCCRA                   TCCR3A
#define PWM_2_TCCRB                   TCCR3B

// ADC
#define ADC_NUM_PINS                  3
#define ADC_ADCSRA                    0x87 // Enable ADC, prescaler /128
#define ADC_DIDR0                     0x00
#define ADC_DIDR1                     0x07
#define ADC_0_NAME                    PSTR("Temp Sens 1")
#define ADC_0_CONVERTER               SteinhartHartBOnlyConverter
#define ADC_ADMUX_0                   0x40
#define ADC_ADCSRB_0                  0x08
#define ADC_1_NAME                    PSTR("Temp Sens 2")
#define ADC_1_CONVERTER               SteinhartHartBOnlyConverter
#define ADC_ADMUX_1                   0x41
#define ADC_ADCSRB_1                  0x08
#define ADC_2_NAME                    PSTR("Temp Sens 3")
#define ADC_2_CONVERTER               SteinhartHartBOnlyConverter
#define ADC_ADMUX_2                   0x42
#define ADC_ADCSRB_2                  0x08

#endif /* HAL_MICROCHIP_AVR_ULTIMAKER_ORIGINAL_BOARD_CFG_H_ */
