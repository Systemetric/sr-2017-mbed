/* pwm_constants.h
 *
 * Copyright 2016 Josh Holland
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef JDH_16_17_RASPI3_PWM_CONSTANTS_H_
#define JDH_16_17_RASPI3_PWM_CONSTANTS_H_

// Datasheet: <https://www.nxp.com/documents/user_manual/UM10360.pdf>
// See page 518 for details on the PWM.
// The following page shows the internal workings of the PWM on the LPC1768
// (the mbed we use, as of 2016/17) -- could be useful if we ever get unexpected
// behaviour when messing around with interrupts.
// <https://developer.mbed.org/users/mbed_official/code/mbed-dev/file/02e0a0aed4ec/targets/TARGET_NXP/TARGET_LPC176X/pwmout_api.c>
// The documentation for those functions can be found here:
// <https://developer.mbed.org/users/mbed_official/code/mbed-dev/file/02e0a0aed4ec/hal/pwmout_api.h>
// NB: *DO NOT* use the the HAL functions in user code. They are subject to
// change at any time.
// This page is also interesting: <https://developer.mbed.org/handbook/mbed-library-internals>

// Interrupt flags for PWM IR (Interrupt Register) (datasheet section 24.6.1).
// These can be used to check if an interrupt is set by masking the IR with AND,
// or to clear an interrupt by writing them to the IR:
//     if ((LPC_PWM1->IR & PWM_MR4) == PWM_MR4) {  // interrupt from channel 4
//         LPC_PWM1->IR = PWM_MR4;  // clear the interrupt
//     }
// Note that writing zeroes to the IR has no effect, so a plain = can be used.
#define PWM_IR_MR0 1<<0  /* Interrupt every period */
#define PWM_IR_MR1 1<<1  /* CH1 = p26 */
#define PWM_IR_MR2 1<<2  /* CH2 = p25 */
#define PWM_IR_MR3 1<<3  /* CH3 = p24 */
#define PWM_IR_CAP0 1<<4
#define PWM_IR_CAP1 1<<5
// bits 6 and 7 are reserved
#define PWM_IR_MR4 1<<8  /* CH4 = p23 */
#define PWM_IR_MR5 1<<9  /* CH5 = p22 */
#define PWM_IR_MR6 1<<10 /* CH6 = p21 */
// bits 11 to 31 are reserved
#define PWM_IR_ALL 1<<0 | 1<<1 | 1<<2 | 1<<3 | 1<<4 | 1<<5 | 1<<8 | 1<<9 | 1<<10

// Interrupt bit locations for PWM MCR (Match Control Register) (datasheet section 24.6.4)
// These can be used to enable and disable interrupts for the different PWM
// match channels by ORing them with the MCR:
//     LPC_PWM1->MCR |= PWMMR4I;  // enable interrupt for channel 4
#define PWM_MR0_INTERRUPT 1<<0
#define PWM_MR1_INTERRUPT 1<<3
#define PWM_MR2_INTERRUPT 1<<6
#define PWM_MR3_INTERRUPT 1<<9
#define PWM_MR4_INTERRUPT 1<<12
#define PWM_MR5_INTERRUPT 1<<15
#define PWM_MR6_INTERRUPT 1<<18

#endif  // JDH_16_17_RASPI3_PWM_CONSTANTS_H_