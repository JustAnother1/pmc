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

/* Timers:
 *  Timer 0 (8 bit):
 *  a: millisecond timer
 *  b: PWM 6 ( Expansion)
 *
 *  Timer 1 (16 bit): Stepper Timer
 *  a:
 *  b:
 *  c:
 *
 *  Timer 2 (8 bit):
 *  a:
 *  b: PWM 0
 *
 *  Timer 3 (16 bit):
 *  a: PWM 7 ( Expansion)
 *  b: PWM 1
 *  c: PWM 2
 *
 *  Timer 4 (16 bit):
 *  a: PWM 3
 *  b: PWM 5
 *  c: PWM 4
 *
 *  Timer 5 (16 bit):
 *  a: Buzzer
 *  b:
 *  c:
 */

// LED - PB7
#define DEBUG_LED_MASK                0x80
#define DEBUG_LED_DDR                 DDRB
#define DEBUG_LED_PIN                 PINB
#define DEBUG_LED_PORT                PORTB

// USART
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

// Step Timer Timer 1 (Channel a)
#define STEP_TIMER_TCCRA              TCCR1A
#define STEP_TIMER_TCCRB              TCCR1B
#define STEP_TIMER_OCRA               OCR1A
#define STEP_TIMER_TCCRA_1            0x00
#define STEP_TIMER_TCCRA_0            0xff
#define STEP_TIMER_TCCRB_1            0x09
#define STEP_TIMER_TCCRB_0            0xf6
#define STEP_TIMER_TIMSK              TIMSK1
#define STEP_TIMER_TIMSK_VALUE        0x02

// Time Base (millisecond timer) -> Timer 0 channel A
#define CPU_MS_TIMER_TCCRA            TCCR0A
#define CPU_MS_TIMER_TCCRA_1          0x03
#define CPU_MS_TIMER_TCCRA_0          0xc0
#define CPU_MS_TIMER_TCCRB            TCCR0B
#define CPU_MS_TIMER_TCCRB_1          0x01
#define CPU_MS_TIMER_TCCRB_0          0xce
#define CPU_MS_TIMER_TCNT             TCNT0
#define CPU_MS_TIMER_OCRA             OCR0A
#define CPU_MS_TIMER_TIMSK            TIMSK0
#define CPU_MS_TIMER_IRQ_1            0x02
#define CPU_MS_TIMER_IRQ_0            0
#define CPU_MS_TIMER_NEEDS_CNT        64
#define CPU_MS_TIMER_RELOAD_VALUE     (uint8_t)((F_CPU/1000)/CPU_MS_TIMER_NEEDS_CNT)
#define CPU_MS_TIMER_COMPARE_ISR      TIMER0_COMPA_vect

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

// Timer 2 Channel B
#define PWM_0_NAME                    "Heated Bed"
#define PWM_0_OCR                     OCR2B
#define PWM_0_TCNT                    TCNT2
#define PWM_0_TIMSK                   TIMSK2
#define PWM_0_TCCRA                   TCCR2A
#define PWM_0_TCCRA_1                 0x23
#define PWM_0_TCCRA_0                 0x10
#define PWM_0_TCCRB                   TCCR2B
#define PWM_0_TCCRB_1                 0x01
#define PWM_0_TCCRB_0                 0xce

// Timer 3 Channel B
#define PWM_1_NAME                    "Heater 0"
#define PWM_1_OCR                     OCR3B
#define PWM_1_TCNT                    TCNT3
#define PWM_1_TIMSK                   TIMSK3
#define PWM_1_TCCRA                   TCCR3A
#define PWM_1_TCCRA_1                 0x21
#define PWM_1_TCCRA_0                 0x12
#define PWM_1_TCCRB                   TCCR3B
#define PWM_1_TCCRB_1                 0x09
#define PWM_1_TCCRB_0                 0x16

// Timer 3 Channel C
#define PWM_2_NAME                    "Heater 1"
#define PWM_2_OCR                     OCR3C
#define PWM_2_TCNT                    TCNT3
#define PWM_2_TIMSK                   TIMSK3
#define PWM_2_TCCRA                   TCCR3A
#define PWM_2_TCCRA_1                 0x09
#define PWM_2_TCCRA_0                 0x06
#define PWM_2_TCCRB                   TCCR3B
#define PWM_2_TCCRB_1                 0x09
#define PWM_2_TCCRB_0                 0x16

// Timer 4 Channel A
#define PWM_3_NAME                    "Heater 2"
#define PWM_3_OCR                     OCR4A
#define PWM_3_TCNT                    TCNT4
#define PWM_3_TIMSK                   TIMSK4
#define PWM_3_TCCRA                   TCCR4A
#define PWM_3_TCCRA_1                 0x81
#define PWM_3_TCCRA_0                 0x42
#define PWM_3_TCCRB                   TCCR4B
#define PWM_3_TCCRB_1                 0x09
#define PWM_3_TCCRB_0                 0x16

// Timer 4 Channel C
#define PWM_4_NAME                    "Fan 0"
#define PWM_4_OCR                     OCR4C
#define PWM_4_TCNT                    TCNT4
#define PWM_4_TIMSK                   TIMSK4
#define PWM_4_TCCRA                   TCCR4A
#define PWM_4_TCCRA_1                 0x09
#define PWM_4_TCCRA_0                 0x06
#define PWM_4_TCCRB                   TCCR4B
#define PWM_4_TCCRB_1                 0x09
#define PWM_4_TCCRB_0                 0x16

// Timer 4 Channel B
#define PWM_5_NAME                    "Fan 1"
#define PWM_5_OCR                     OCR4B
#define PWM_5_TCNT                    TCNT4
#define PWM_5_TIMSK                   TIMSK4
#define PWM_5_TCCRA                   TCCR4A
#define PWM_5_TCCRA_1                 0x21
#define PWM_5_TCCRA_0                 0x12
#define PWM_5_TCCRB                   TCCR4B
#define PWM_5_TCCRB_1                 0x09
#define PWM_5_TCCRB_0                 0x16

// Timer 0 Channel B
#define PWM_6_NAME                    "Expansion 1"
#define PWM_6_OCR                     OCR0B
#define PWM_6_TCNT                    TCNT0
#define PWM_6_TIMSK                   TIMSK0
#define PWM_6_TCCRA                   TCCR0A
#define PWM_6_TCCRA_1                 0x23
#define PWM_6_TCCRA_0                 0x10
#define PWM_6_TCCRB                   TCCR0B
#define PWM_6_TCCRB_1                 0x01
#define PWM_6_TCCRB_0                 0xce

// Timer 3 Channel A
#define PWM_7_NAME                    "Expansion 2"
#define PWM_7_OCR                     OCR3A
#define PWM_7_TCNT                    TCNT3
#define PWM_7_TIMSK                   TIMSK3
#define PWM_7_TCCRA                   TCCR3A
#define PWM_7_TCCRA_1                 0x81
#define PWM_7_TCCRA_0                 0x42
#define PWM_7_TCCRB                   TCCR3B
#define PWM_7_TCCRB_1                 0x09
#define PWM_7_TCCRB_0                 0x16

// Buzzer
#define BUZZER_0_NAME                 "Buzzer"
#define BUZZER_PORT_1                 PORTL
#define BUZZER_DD_1                   DDRL
#define BUZZER_PORT_1_MASK            0x20; // L5
// Timer 5 Channel A
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
#define ADC_0_NAME                    "Temp Sens 1"
#define ADC_0_CONVERTER               SteinhartHartBOnlyConverter
#define ADC_ADMUX_0                   0x47
#define ADC_ADCSRB_0                  0x08
#define ADC_1_NAME                    "Temp Sens 2"
#define ADC_1_CONVERTER               SteinhartHartBOnlyConverter
#define ADC_ADMUX_1                   0x46
#define ADC_ADCSRB_1                  0x08
#define ADC_2_NAME                    "Temp Sens 3"
#define ADC_2_CONVERTER               SteinhartHartBOnlyConverter
#define ADC_ADMUX_2                   0x45
#define ADC_ADCSRB_2                  0x08
#define ADC_3_NAME                    "Temp Sens 4"
#define ADC_3_CONVERTER               SteinhartHartBOnlyConverter
#define ADC_ADMUX_3                   0x44
#define ADC_ADCSRB_3                  0x08
#define ADC_4_NAME                    "Temp Sens 5"
#define ADC_4_CONVERTER               NoConverter
#define ADC_ADMUX_4                   0x43
#define ADC_ADCSRB_4                  0x08

// Digital Input Pins

#define D_IN_NUM_PINS                 6
// C0
#define D_IN_0_NAME                   "X-Min"
#define D_IN_0_DDR                    DDRC
#define D_IN_0_PORT                   PORTC
#define D_IN_0_PIN                    PINC
#define D_IN_0_MASK                   0x01
// C1
#define D_IN_1_NAME                   "X-Max"
#define D_IN_1_DDR                    DDRC
#define D_IN_1_PORT                   PORTC
#define D_IN_1_PIN                    PINC
#define D_IN_1_MASK                   0x02
// C2
#define D_IN_2_NAME                   "Y-Min"
#define D_IN_2_DDR                    DDRC
#define D_IN_2_PORT                   PORTC
#define D_IN_2_PIN                    PINC
#define D_IN_2_MASK                   0x04
// C3
#define D_IN_3_NAME                   "Y-Max"
#define D_IN_3_DDR                    DDRC
#define D_IN_3_PORT                   PORTC
#define D_IN_3_PIN                    PINC
#define D_IN_3_MASK                   0x08
// C4
#define D_IN_4_NAME                   "Z-Min"
#define D_IN_4_DDR                    DDRC
#define D_IN_4_PORT                   PORTC
#define D_IN_4_PIN                    PINC
#define D_IN_4_MASK                   0x10
// C5
#define D_IN_5_NAME                   "Z-Max"
#define D_IN_5_DDR                    DDRC
#define D_IN_5_PORT                   PORTC
#define D_IN_5_PIN                    PINC
#define D_IN_5_MASK                   0x20

// Pololu Stepper

#define STEPPER_0_NAME                "X"
// Enable : PL1
#define STEPPER_0_ENABLE_DDR          DDRL
#define STEPPER_0_ENABLE_PORT         PORTL
#define STEPPER_0_ENABLE_MASK         0x02
// Step : PH0
#define STEPPER_0_STEP_DDR            DDRH
#define STEPPER_0_STEP_PORT           PORTH
#define STEPPER_0_STEP_MASK           0x01
// Dir : PH1
#define STEPPER_0_DIR_DDR             DDRH
#define STEPPER_0_DIR_PORT            PORTH
#define STEPPER_0_DIR_MASK            0x02

#define STEPPER_1_NAME                "Y"
// Enable : PF1
#define STEPPER_1_ENABLE_DDR          DDRF
#define STEPPER_1_ENABLE_PORT         PORTF
#define STEPPER_1_ENABLE_MASK         0x02
// Step : PF0
#define STEPPER_1_STEP_DDR            DDRF
#define STEPPER_1_STEP_PORT           PORTF
#define STEPPER_1_STEP_MASK           0x01
// Dir : PL2
#define STEPPER_1_DIR_DDR             DDRL
#define STEPPER_1_DIR_PORT            PORTL
#define STEPPER_1_DIR_MASK            0x04

#define STEPPER_2_NAME                "Z"
// Enable : PK0
#define STEPPER_2_ENABLE_DDR          DDRK
#define STEPPER_2_ENABLE_PORT         PORTK
#define STEPPER_2_ENABLE_MASK         0x01
// Step : PF3
#define STEPPER_2_STEP_DDR            DDRF
#define STEPPER_2_STEP_PORT           PORTF
#define STEPPER_2_STEP_MASK           0x08
// Dir : PF2
#define STEPPER_2_DIR_DDR             DDRF
#define STEPPER_2_DIR_PORT            PORTF
#define STEPPER_2_DIR_MASK            0x04

#define STEPPER_3_NAME                "E0"
// Enable : PA2
#define STEPPER_3_ENABLE_DDR          DDRA
#define STEPPER_3_ENABLE_PORT         PORTA
#define STEPPER_3_ENABLE_MASK         0x04
// Step : PA1
#define STEPPER_3_STEP_DDR            DDRA
#define STEPPER_3_STEP_PORT           PORTA
#define STEPPER_3_STEP_MASK           0x02
// Dir : PA0
#define STEPPER_3_DIR_DDR             DDRA
#define STEPPER_3_DIR_PORT            PORTA
#define STEPPER_3_DIR_MASK            0x01

#define STEPPER_4_NAME                "E1"
// Enable : PA5
#define STEPPER_4_ENABLE_DDR          DDRA
#define STEPPER_4_ENABLE_PORT         PORTA
#define STEPPER_4_ENABLE_MASK         0x20
// Step : PA4
#define STEPPER_4_STEP_DDR            DDRA
#define STEPPER_4_STEP_PORT           PORTA
#define STEPPER_4_STEP_MASK           0x10
// Dir : PA3
#define STEPPER_4_DIR_DDR             DDRA
#define STEPPER_4_DIR_PORT            PORTA
#define STEPPER_4_DIR_MASK            0x08

#define STEPPER_5_NAME                "E2"
// Enable : PG2
#define STEPPER_5_ENABLE_DDR          DDRG
#define STEPPER_5_ENABLE_PORT         PORTG
#define STEPPER_5_ENABLE_MASK         0x04
// Step : PA7
#define STEPPER_5_STEP_DDR            DDRA
#define STEPPER_5_STEP_PORT           PORTA
#define STEPPER_5_STEP_MASK           0x80
// Dir : PA6
#define STEPPER_5_DIR_DDR             DDRA
#define STEPPER_5_DIR_PORT            PORTA
#define STEPPER_5_DIR_MASK            0x040


#endif /* HAL_MICROCHIP_AVR_RUMBA_BOARD_CFG_H_ */
