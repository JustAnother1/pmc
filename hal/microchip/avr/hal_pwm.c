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

#include "board_cfg.h"
#include "hal_cfg.h"
#include "hal_pwm.h"

uint8_t cur_pwm[PWM_NUM_PINS];

void hal_pwm_init(void)
{
    int i;
    // Output a Low(0) for a start
    PWM_PORT_1 &= ~PWM_PORT_1_MASK;
#if PWM_NUM_PORTS > 1
    PWM_PORT_2 &= ~PWM_PORT_2_MASK;
#endif
#if PWM_NUM_PORTS > 2
    PWM_PORT_3 &= ~PWM_PORT_3_MASK;
#endif
    // Set the Data Direction on the used pins
    PWM_DD_1 |= PWM_PORT_1_MASK;
#if PWM_NUM_PORTS > 1
    PWM_DD_2 |= PWM_PORT_2_MASK;
#endif
#if PWM_NUM_PORTS > 2
    PWM_DD_3 |= PWM_PORT_3_MASK;
#endif
    // initialize variables to all PWM disabled
    for(i = 0; i < PWM_NUM_PINS; i++)
    {
        cur_pwm[i] = 0;
    }
    // Disable timers
    PWM_0_TCCRB = 0;
#if PWM_NUM_PINS > 1
    PWM_1_TCCRB = 0;
#endif
#if PWM_NUM_PINS > 2
    PWM_2_TCCRB = 0;
#endif
#if PWM_NUM_PINS > 3
    PWM_3_TCCRB = 0;
#endif
#if PWM_NUM_PINS > 4
    PWM_4_TCCRB = 0;
#endif
#if PWM_NUM_PINS > 5
    PWM_5_TCCRB = 0;
#endif
#if PWM_NUM_PINS > 6
    PWM_6_TCCRB = 0;
#endif
#if PWM_NUM_PINS > 7
    PWM_7_TCCRB = 0;
#endif
}

uint_fast8_t hal_pwm_get_amount(void)
{
    return PWM_NUM_PINS - NUMBER_OF_HEATERS;
}

void hal_pwm_set_on_time(uint_fast8_t device, uint_fast16_t on_time)
{
    switch(device)
    {
    case 0:
        PWM_0_OCR   = on_time>>8;
        PWM_0_TIMSK = 0x00; // no Interrupts !
        PWM_0_TCCRA = 0xa9; // non inverted PWM output
        PWM_0_TCNT  = 0;
        PWM_0_TCCRB = 0x09; // No prescaler - CTC
        break;
#if PWM_NUM_PINS > 1
    case 1:
        PWM_1_OCR   = on_time>>8;
        PWM_1_TIMSK = 0x00; // no Interrupts !
        PWM_1_TCCRA = 0xa9; // non inverted PWM output
        PWM_1_TCNT  = 0;
        PWM_1_TCCRB = 0x09; // No prescaler - CTC
        break;
#endif
#if PWM_NUM_PINS > 2
    case 2:
        PWM_2_OCR   = on_time>>8;
        PWM_2_TIMSK = 0x00; // no Interrupts !
        PWM_2_TCCRA = 0xa9; // non inverted PWM output
        PWM_2_TCNT  = 0;
        PWM_2_TCCRB = 0x09; // No prescaler - CTC
        break;
#endif
#if PWM_NUM_PINS > 3
    case 3:
        PWM_3_OCR   = on_time>>8;
        PWM_3_TIMSK = 0x00; // no Interrupts !
        PWM_3_TCCRA = 0xa9; // non inverted PWM output
        PWM_3_TCNT  = 0;
        PWM_3_TCCRB = 0x09; // No prescaler - CTC
        break;
#endif
#if PWM_NUM_PINS > 4
    case 4:
        PWM_4_OCR   = on_time>>8;
        PWM_4_TIMSK = 0x00; // no Interrupts !
        PWM_4_TCCRA = 0xa9; // non inverted PWM output
        PWM_4_TCNT  = 0;
        PWM_4_TCCRB = 0x09; // No prescaler - CTC
        break;
#endif
#if PWM_NUM_PINS > 5
    case 5:
        PWM_5_OCR   = on_time>>8;
        PWM_5_TIMSK = 0x00; // no Interrupts !
        PWM_5_TCCRA = 0xa9; // non inverted PWM output
        PWM_5_TCNT  = 0;
        PWM_5_TCCRB = 0x09; // No prescaler - CTC
        break;
#endif
#if PWM_NUM_PINS > 6
    case 6:
        PWM_6_OCR   = on_time>>8;
        PWM_6_TIMSK = 0x00; // no Interrupts !
        PWM_6_TCCRA = 0xa9; // non inverted PWM output
        PWM_6_TCNT  = 0;
        PWM_6_TCCRB = 0x09; // No prescaler - CTC
        break;
#endif
#if PWM_NUM_PINS > 7
    case 7:
        PWM_7_OCR   = on_time>>8;
        PWM_7_TIMSK = 0x00; // no Interrupts !
        PWM_7_TCCRA = 0xa9; // non inverted PWM output
        PWM_7_TCNT  = 0;
        PWM_7_TCCRB = 0x09; // No prescaler - CTC
        break;
#endif
    default:
        // Invalid device !
        break;
    }
}

uint_fast8_t hal_pwm_get_name(uint_fast8_t device, uint8_t *position, uint_fast8_t max_length)
{
    switch(device)
    {
#if PWM_NUM_PINS > 0
    case  0: return strlcpy_P((char *)position, PWM_0_NAME, max_length);
#endif
#if PWM_NUM_PINS > 1
    case  1: return strlcpy_P((char *)position, PWM_1_NAME, max_length);
#endif
#if PWM_NUM_PINS > 2
    case  2: return strlcpy_P((char *)position, PWM_2_NAME, max_length);
#endif
#if PWM_NUM_PINS > 3
    case  3: return strlcpy_P((char *)position, PWM_3_NAME, max_length);
#endif
#if PWM_NUM_PINS > 4
    case  4: return strlcpy_P((char *)position, PWM_4_NAME, max_length);
#endif
#if PWM_NUM_PINS > 5
    case  5: return strlcpy_P((char *)position, PWM_5_NAME, max_length);
#endif
#if PWM_NUM_PINS > 6
    case  6: return strlcpy_P((char *)position, PWM_6_NAME, max_length);
#endif
#if PWM_NUM_PINS > 7
    case  7: return strlcpy_P((char *)position, PWM_7_NAME, max_length);
#endif
    default:
        return 0;
    }
}

