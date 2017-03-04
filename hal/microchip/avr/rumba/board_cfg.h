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

#include <avr/io.h>
#include <avr/pgmspace.h>

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
#define CPU_MS_TIMER_RELOAD_VALUE     (F_CPU/1000)
#define CPU_MS_TIMER_COMPARE_A_ISR    TIMER1_COMPA_vect
#define CPU_MS_TIMER_OVERFLOW_ISR     TIMER1_OVF_vect

// PWM
#define PWM_NUM_PINS                  8
#define PWM_NUM_PORTS                 3
#define PWM_PORT_1                    PORTE
#define PWM_DD_1                      DDRE
#define PWM_PORT_1_MASK               0x38; // E3, E4; E5
#define PWM_PORT_2                    PORTG
#define PWM_DD_2                      DDRG
#define PWM_PORT_2_MASK               0x20; // G5
#define PWM_PORT_3                    PORTH
#define PWM_DD_3                      DDRH
#define PWM_PORT_3_MASK               0x78; // H3, H4; H5, H6

#define PWM_0_NAME                    PSTR("Heated Bed")
#define PWM_0_OCR                     OCR2B
#define PWM_0_TCNT                    TCNT2
#define PWM_0_TIMSK                   TIMSK2
#define PWM_0_TCCRA                   TCCR2A
#define PWM_0_TCCRB                   TCCR2B

#define PWM_1_NAME                    PSTR("Heater 0")
#define PWM_1_OCR                     OCR3B
#define PWM_1_TCNT                    TCNT3
#define PWM_1_TIMSK                   TIMSK3
#define PWM_1_TCCRA                   TCCR3A
#define PWM_1_TCCRB                   TCCR3B

#define PWM_2_NAME                    PSTR("Heater 1")
#define PWM_2_OCR                     OCR3C
#define PWM_2_TCNT                    TCNT3
#define PWM_2_TIMSK                   TIMSK3
#define PWM_2_TCCRA                   TCCR3A
#define PWM_2_TCCRB                   TCCR3B

#define PWM_3_NAME                    PSTR("Heater 2")
#define PWM_3_OCR                     OCR4A
#define PWM_3_TCNT                    TCNT4
#define PWM_3_TIMSK                   TIMSK4
#define PWM_3_TCCRA                   TCCR4A
#define PWM_3_TCCRB                   TCCR4B

#define PWM_4_NAME                    PSTR("Fan 0")
#define PWM_4_OCR                     OCR4C
#define PWM_4_TCNT                    TCNT4
#define PWM_4_TIMSK                   TIMSK4
#define PWM_4_TCCRA                   TCCR4A
#define PWM_4_TCCRB                   TCCR4B

#define PWM_5_NAME                    PSTR("Fan 1")
#define PWM_5_OCR                     OCR4B
#define PWM_5_TCNT                    TCNT4
#define PWM_5_TIMSK                   TIMSK4
#define PWM_5_TCCRA                   TCCR4A
#define PWM_5_TCCRB                   TCCR4B

#define PWM_6_NAME                    PSTR("Expansion 1")
#define PWM_6_OCR                     OCR0B
#define PWM_6_TCNT                    TCNT0
#define PWM_6_TIMSK                   TIMSK0
#define PWM_6_TCCRA                   TCCR0A
#define PWM_6_TCCRB                   TCCR0B

#define PWM_7_NAME                    PSTR("Expansion 2")
#define PWM_7_OCR                     OCR3A
#define PWM_7_TCNT                    TCNT3
#define PWM_7_TIMSK                   TIMSK3
#define PWM_7_TCCRA                   TCCR3A
#define PWM_7_TCCRB                   TCCR3B

// Buzzer
#define BUZZER_0_NAME                 PSTR("Buzzer")
#define BUZZER_PORT_1                 PORTL
#define BUZZER_DD_1                   DDRL
#define BUZZER_PORT_1_MASK            0x20; // L5
#define BUZZER_0_OCRA                 OCR5A
#define BUZZER_0_TCCRA                TCCR5A
#define BUZZER_0_TCCRB                TCCR5B
#define BUZZER_0_TIMSK                TIMSK5
#define BUZZER_0_TCNT                 TCNT5

// ADC
#define ADC_NUM_PINS                  5
#define ADC_ADCSRA                    0x87 // Enable ADC, prescaler /128
#define ADC_DIDR0                     0x00
#define ADC_DIDR1                     0xf8
#define ADC_0_NAME                    PSTR("Temp Sens 1")
#define ADC_0_CONVERTER               SteinhartHartBOnlyConverter
#define ADC_ADMUX_0                   0x47
#define ADC_ADCSRB_0                  0x08
#define ADC_1_NAME                    PSTR("Temp Sens 2")
#define ADC_1_CONVERTER               SteinhartHartBOnlyConverter
#define ADC_ADMUX_1                   0x46
#define ADC_ADCSRB_1                  0x08
#define ADC_2_NAME                    PSTR("Temp Sens 3")
#define ADC_2_CONVERTER               SteinhartHartBOnlyConverter
#define ADC_ADMUX_2                   0x45
#define ADC_ADCSRB_2                  0x08
#define ADC_3_NAME                    PSTR("Temp Sens 4")
#define ADC_3_CONVERTER               SteinhartHartBOnlyConverter
#define ADC_ADMUX_3                   0x44
#define ADC_ADCSRB_3                  0x08
#define ADC_4_NAME                    PSTR("Temp Sens 5")
#define ADC_4_CONVERTER               NoConverter
#define ADC_ADMUX_4                   0x43
#define ADC_ADCSRB_4                  0x08


#endif /* HAL_MICROCHIP_AVR_RUMBA_BOARD_CFG_H_ */
