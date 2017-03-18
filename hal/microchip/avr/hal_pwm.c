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
#include "hal_debug.h"

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
    // Do not disable timers !
    // Not all Timers are exclusively used for PWM generation.
    // the time base (millisecond tick) and the stepper also use timers!
    // we must not interfere with those usages.
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
        cur_pwm[0] = on_time>>8;
        PWM_0_OCR   = cur_pwm[0];
        PWM_0_TCCRA = (PWM_0_TCCRA | PWM_0_TCCRA_1) &~PWM_0_TCCRA_0;
        PWM_0_TCCRB = (PWM_0_TCCRB | PWM_0_TCCRB_1) &~PWM_0_TCCRB_0;
        break;
#if PWM_NUM_PINS > 1
    case 1:
        cur_pwm[1] = on_time>>8;
        PWM_1_OCR   = cur_pwm[1];
        PWM_1_TCCRA = (PWM_1_TCCRA | PWM_1_TCCRA_1) &~PWM_1_TCCRA_0;
        PWM_1_TCCRB = (PWM_1_TCCRB | PWM_1_TCCRB_1) &~PWM_1_TCCRB_0;
        break;
#endif
#if PWM_NUM_PINS > 2
    case 2:
        cur_pwm[2] = on_time>>8;
        PWM_2_OCR   = cur_pwm[2];
        PWM_2_TCCRA = (PWM_2_TCCRA | PWM_2_TCCRA_1) &~PWM_2_TCCRA_0;
        PWM_2_TCCRB = (PWM_2_TCCRB | PWM_2_TCCRB_1) &~PWM_2_TCCRB_0;
        break;
#endif
#if PWM_NUM_PINS > 3
    case 3:
        cur_pwm[3] = on_time>>8;
        PWM_3_OCR   = cur_pwm[3];
        PWM_3_TCCRA = (PWM_3_TCCRA | PWM_3_TCCRA_1) &~PWM_3_TCCRA_0;
        PWM_3_TCCRB = (PWM_3_TCCRB | PWM_3_TCCRB_1) &~PWM_3_TCCRB_0;
        break;
#endif
#if PWM_NUM_PINS > 4
    case 4:
        cur_pwm[4] = on_time>>8;
        PWM_4_OCR   = cur_pwm[4];
        PWM_4_TCCRA = (PWM_4_TCCRA | PWM_4_TCCRA_1) &~PWM_4_TCCRA_0;
        PWM_4_TCCRB = (PWM_4_TCCRB | PWM_4_TCCRB_1) &~PWM_4_TCCRB_0;
        break;
#endif
#if PWM_NUM_PINS > 5
    case 5:
        cur_pwm[5] = on_time>>8;
        PWM_5_OCR   = cur_pwm[5];
        PWM_5_TCCRA = (PWM_5_TCCRA | PWM_5_TCCRA_1) &~PWM_5_TCCRA_0;
        PWM_5_TCCRB = (PWM_5_TCCRB | PWM_5_TCCRB_1) &~PWM_5_TCCRB_0;
        break;
#endif
#if PWM_NUM_PINS > 6
    case 6:
        cur_pwm[6] = on_time>>8;
        PWM_6_OCR   = cur_pwm[6];
        PWM_6_TCCRA = (PWM_6_TCCRA | PWM_6_TCCRA_1) &~PWM_6_TCCRA_0;
        PWM_6_TCCRB = (PWM_6_TCCRB | PWM_6_TCCRB_1) &~PWM_6_TCCRB_0;
        break;
#endif
#if PWM_NUM_PINS > 7
    case 7:
        cur_pwm[7] = on_time>>8;
        PWM_7_OCR   = cur_pwm[7];
        PWM_7_TCCRA = (PWM_7_TCCRA | PWM_7_TCCRA_1) &~PWM_7_TCCRA_0;
        PWM_7_TCCRB = (PWM_7_TCCRB | PWM_7_TCCRB_1) &~PWM_7_TCCRB_0;
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
    case  0: return strlcpy_P((char *)position, STR(PWM_0_NAME), max_length);
#endif
#if PWM_NUM_PINS > 1
    case  1: return strlcpy_P((char *)position, STR(PWM_1_NAME), max_length);
#endif
#if PWM_NUM_PINS > 2
    case  2: return strlcpy_P((char *)position, STR(PWM_2_NAME), max_length);
#endif
#if PWM_NUM_PINS > 3
    case  3: return strlcpy_P((char *)position, STR(PWM_3_NAME), max_length);
#endif
#if PWM_NUM_PINS > 4
    case  4: return strlcpy_P((char *)position, STR(PWM_4_NAME), max_length);
#endif
#if PWM_NUM_PINS > 5
    case  5: return strlcpy_P((char *)position, STR(PWM_5_NAME), max_length);
#endif
#if PWM_NUM_PINS > 6
    case  6: return strlcpy_P((char *)position, STR(PWM_6_NAME), max_length);
#endif
#if PWM_NUM_PINS > 7
    case  7: return strlcpy_P((char *)position, STR(PWM_7_NAME), max_length);
#endif
    default:
        return 0;
    }
}

#ifdef DEBUG_ACTIVE
bool hal_pwm_print_configuration(int pwmNumber, int lineNumber)
{
    if(0 == lineNumber)
    {
        debug_line(STR("PWM_DD_1    = 0x%02x"), PWM_DD_1);
        return true;
    }
    if(1 == lineNumber)
    {
        debug_line(STR("PWM_DD_2    = 0x%02x"), PWM_DD_2);
        return true;
    }
    if(2 == lineNumber)
    {
        debug_line(STR("PWM_DD_3    = 0x%02x"), PWM_DD_3);
        return true;
    }
    if(3 == lineNumber)
    {
        debug_line(STR("PWM_PORT_1  = 0x%02x"), PWM_PORT_1);
        return true;
    }
    if(4 == lineNumber)
    {
        debug_line(STR("PWM_PORT_1  = 0x%02x"), PWM_PORT_2);
        return true;
    }
    if(5 == lineNumber)
    {
        debug_line(STR("PWM_PORT_1  = 0x%02x"), PWM_PORT_3);
        return true;
    }
    switch(pwmNumber)
    {
#if PWM_NUM_PINS > 0
    case  0:
        if(6 == lineNumber)
        {
            debug_line(STR("PWM_0_OCR   = 0x%02x"), PWM_0_OCR);
            return true;
        }
        if(7 == lineNumber)
        {
            debug_line(STR("PWM_0_TIMSK = 0x%02x"), PWM_0_TIMSK);
            return true;
        }
        if(8 == lineNumber)
        {
            debug_line(STR("PWM_0_TCCRA = 0x%02x"), PWM_0_TCCRA);
            return true;
        }
        if(9 == lineNumber)
        {
            debug_line(STR("PWM_0_TCNT  = 0x%04x"), PWM_0_TCNT);
            return true;
        }
        if(10 == lineNumber)
        {
            debug_line(STR("PWM_0_TCCRB = 0x%02x"), PWM_0_TCCRB);
            return true;
        }
        if(11 == lineNumber)
        {
            debug_line(STR("cur_pwm[0]  = 0x%02x"), cur_pwm[0]);
            return true;
        }
        break;
#endif
#if PWM_NUM_PINS > 1
    case  1:
        if(6 == lineNumber)
        {
            debug_line(STR("PWM_1_OCR   = 0x%02x"), PWM_1_OCR);
            return true;
        }
        if(7 == lineNumber)
        {
            debug_line(STR("PWM_1_TIMSK = 0x%02x"), PWM_1_TIMSK);
            return true;
        }
        if(8 == lineNumber)
        {
            debug_line(STR("PWM_1_TCCRA = 0x%02x"), PWM_1_TCCRA);
            return true;
        }
        if(9 == lineNumber)
        {
            debug_line(STR("PWM_1_TCNT  = 0x%04x"), PWM_1_TCNT);
            return true;
        }
        if(10 == lineNumber)
        {
            debug_line(STR("PWM_1_TCCRB = 0x%02x"), PWM_1_TCCRB);
            return true;
        }
        if(11 == lineNumber)
        {
            debug_line(STR("cur_pwm[1]  = 0x%02x"), cur_pwm[1]);
            return true;
        }
        break;
#endif
#if PWM_NUM_PINS > 2
    case  2:
        if(6 == lineNumber)
        {
            debug_line(STR("PWM_2_OCR   = 0x%02x"), PWM_2_OCR);
            return true;
        }
        if(7 == lineNumber)
        {
            debug_line(STR("PWM_2_TIMSK = 0x%02x"), PWM_2_TIMSK);
            return true;
        }
        if(8 == lineNumber)
        {
            debug_line(STR("PWM_2_TCCRA = 0x%02x"), PWM_2_TCCRA);
            return true;
        }
        if(9 == lineNumber)
        {
            debug_line(STR("PWM_2_TCNT  = 0x%04x"), PWM_2_TCNT);
            return true;
        }
        if(10 == lineNumber)
        {
            debug_line(STR("PWM_2_TCCRB = 0x%02x"), PWM_2_TCCRB);
            return true;
        }
        if(11 == lineNumber)
        {
            debug_line(STR("cur_pwm[2]  = 0x%02x"), cur_pwm[2]);
            return true;
        }
        break;
#endif
#if PWM_NUM_PINS > 3
    case  3:
        if(6 == lineNumber)
        {
            debug_line(STR("PWM_3_OCR   = 0x%02x"), PWM_3_OCR);
            return true;
        }
        if(7 == lineNumber)
        {
            debug_line(STR("PWM_3_TIMSK = 0x%02x"), PWM_3_TIMSK);
            return true;
        }
        if(8 == lineNumber)
        {
            debug_line(STR("PWM_3_TCCRA = 0x%02x"), PWM_3_TCCRA);
            return true;
        }
        if(9 == lineNumber)
        {
            debug_line(STR("PWM_3_TCNT  = 0x%04x"), PWM_3_TCNT);
            return true;
        }
        if(10 == lineNumber)
        {
            debug_line(STR("PWM_3_TCCRB = 0x%02x"), PWM_3_TCCRB);
            return true;
        }
        if(11 == lineNumber)
        {
            debug_line(STR("cur_pwm[3]  = 0x%02x"), cur_pwm[3]);
            return true;
        }
        break;
#endif
#if PWM_NUM_PINS > 4
    case  4:
        if(6 == lineNumber)
        {
            debug_line(STR("PWM_4_OCR   = 0x%02x"), PWM_4_OCR);
            return true;
        }
        if(7 == lineNumber)
        {
            debug_line(STR("PWM_4_TIMSK = 0x%02x"), PWM_4_TIMSK);
            return true;
        }
        if(8 == lineNumber)
        {
            debug_line(STR("PWM_4_TCCRA = 0x%02x"), PWM_4_TCCRA);
            return true;
        }
        if(9 == lineNumber)
        {
            debug_line(STR("PWM_4_TCNT  = 0x%04x"), PWM_4_TCNT);
            return true;
        }
        if(10 == lineNumber)
        {
            debug_line(STR("PWM_4_TCCRB = 0x%02x"), PWM_4_TCCRB);
            return true;
        }
        if(11 == lineNumber)
        {
            debug_line(STR("cur_pwm[4]  = 0x%02x"), cur_pwm[4]);
            return true;
        }
        break;
#endif
#if PWM_NUM_PINS > 5
    case  5:
        if(6 == lineNumber)
        {
            debug_line(STR("PWM_5_OCR   = 0x%02x"), PWM_5_OCR);
            return true;
        }
        if(7 == lineNumber)
        {
            debug_line(STR("PWM_5_TIMSK = 0x%02x"), PWM_5_TIMSK);
            return true;
        }
        if(8 == lineNumber)
        {
            debug_line(STR("PWM_5_TCCRA = 0x%02x"), PWM_5_TCCRA);
            return true;
        }
        if(9 == lineNumber)
        {
            debug_line(STR("PWM_5_TCNT  = 0x%04x"), PWM_5_TCNT);
            return true;
        }
        if(10 == lineNumber)
        {
            debug_line(STR("PWM_5_TCCRB = 0x%02x"), PWM_5_TCCRB);
            return true;
        }
        if(11 == lineNumber)
        {
            debug_line(STR("cur_pwm[5]  = 0x%02x"), cur_pwm[5]);
            return true;
        }
        break;
#endif
#if PWM_NUM_PINS > 6
    case  6:
        if(6 == lineNumber)
        {
            debug_line(STR("PWM_6_OCR   = 0x%02x"), PWM_6_OCR);
            return true;
        }
        if(7 == lineNumber)
        {
            debug_line(STR("PWM_6_TIMSK = 0x%02x"), PWM_6_TIMSK);
            return true;
        }
        if(8 == lineNumber)
        {
            debug_line(STR("PWM_6_TCCRA = 0x%02x"), PWM_6_TCCRA);
            return true;
        }
        if(9 == lineNumber)
        {
            debug_line(STR("PWM_6_TCNT  = 0x%04x"), PWM_6_TCNT);
            return true;
        }
        if(10 == lineNumber)
        {
            debug_line(STR("PWM_6_TCCRB = 0x%02x"), PWM_6_TCCRB);
            return true;
        }
        if(11 == lineNumber)
        {
            debug_line(STR("cur_pwm[6]  = 0x%02x"), cur_pwm[6]);
            return true;
        }
        break;
#endif
#if PWM_NUM_PINS > 7
    case  7:
        if(6 == lineNumber)
        {
            debug_line(STR("PWM_7_OCR   = 0x%02x"), PWM_7_OCR);
            return true;
        }
        if(7 == lineNumber)
        {
            debug_line(STR("PWM_7_TIMSK = 0x%02x"), PWM_7_TIMSK);
            return true;
        }
        if(8 == lineNumber)
        {
            debug_line(STR("PWM_7_TCCRA = 0x%02x"), PWM_7_TCCRA);
            return true;
        }
        if(9 == lineNumber)
        {
            debug_line(STR("PWM_7_TCNT  = 0x%04x"), PWM_7_TCNT);
            return true;
        }
        if(10 == lineNumber)
        {
            debug_line(STR("PWM_7_TCCRB = 0x%02x"), PWM_7_TCCRB);
            return true;
        }
        if(11 == lineNumber)
        {
            debug_line(STR("cur_pwm[7]  = 0x%02x"), cur_pwm[7]);
            return true;
        }
        break;
#endif
    default:
        debug_line(STR("No Such PWM!"));
        return false;
        break;
    }
    return false;
}
#endif

