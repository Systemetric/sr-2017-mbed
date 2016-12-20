/* main.cpp
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

#include "mbed.h"

#include "robot_specific.h"
#include "interrupt_handlers.h"
#include "pwm_constants.h"

int main() {
    // Initialise the PWM -- this must happen before enabling interrupts or we
    // get weird behaviour until a command has been executed.
    pwm.period_ms(0);
    pwm.pulsewidth_ms(0);
    // Enable interrupts for PWM match channel 4
    // The choice of channel 4 is arbitary; each channel is associated with one
    // of the pins on the mbed, and it happened that pin 23 was available, so we
    // decided to use channel 4. Changing this shouldn't stop the robot working
    // (as long as you physically connect the right pin per pwm.h).
    LPC_PWM1->MCR |= PWM_MR4_INTERRUPT;
    // Attach handler to PWM interrupts
    NVIC_SetVector(PWM1_IRQn, (uint32_t)&PwmHandler);
    // Enable PWM interrupts
    NVIC_EnableIRQ(PWM1_IRQn);

    ticker.attach(&ScaleSpeed, 0.05);

    usb_serial.baud(115200);
    while (usb_serial.readable()) {
         // Drain any spurious/garbage characters, just in case. If the Pi is
         // trying to talk to us this early -- too bad.
         usb_serial.getc();  
    }
    usb_serial.attach(&SerialHandler);
}