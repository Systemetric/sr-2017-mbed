/* main.cpp
 *
 * This file is part of the code for the Hills Road/Systemetric entry to the
 * 2017 Student Robotics competition "Easy as ABC".
 *
 * Written by Josh Holland <anowlcalledjosh@gmail.com>
 *
 * This file is available under CC0. You may use it for any purpose.
 */

#include "mbed.h"

#include "robot_specific.h"
#include "interrupt_handlers.h"
#include "pwm_constants.h"

#include <MPU6050.h>

DigitalOut led_two(LED2);
DigitalOut led_three(LED3);
extern bool GyroTurnDetected = false;
extern int ignore_for = 0;
MPU6050 gyro(p28, p27);
Serial log_mbed(p9, p10);

int main() {
    int oldest = 0;
    int8_t buffer[1024] = { };
    int mean;
    int8_t GyZ, old_value;
    int sum = 0;
    log_mbed.baud(115200);
    
    // Disable both wheels to avoid current overload.
    left_wheel_enable = 0;
    right_wheel_enable = 0;

    // Initialise the PWM -- this must happen before enabling interrupts or we
    // get weird behaviour.
    pwm.period_ms(0);
    pwm.pulsewidth_ms(0);
    // Enable interrupts for PWM match channel 4
    // The choice of channel 4 is arbitary; each channel is associated with one
    // of the pins on the mbed, and it happened that pin 23 was available, so we
    // decided to use channel 4. Changing this shouldn't stop the robot working
    // (as long as you physically connect the right pin per pwm.h).
    LPC_PWM1->MCR |= PWM_MR4_INTERRUPT | PWM_MR0_INTERRUPT;
    // Attach handler to PWM interrupts
    NVIC_SetVector(PWM1_IRQn, (uint32_t)&PwmHandler);
    // Enable PWM interrupts
    NVIC_EnableIRQ(PWM1_IRQn);

    // Enable acceleration -- if you want to disable acceleration (in order to
    // show the robot off publicly), comment out this line.
    ticker.attach(&ScaleSpeed, 0.004);

    usb_serial.baud(115200);
    while (usb_serial.readable()) {
         // Drain any spurious/garbage characters, just in case. If the Pi is
         // trying to talk to us this early -- too bad.
         usb_serial.getc();
    }
    // Attach SerialHandler to the serial receive interrupt.
    usb_serial.attach(&SerialHandler);

    led_two = 1;
    // Wait until the motor boards have power, then enable the motor boards.
    while (battery_voltage.read() < 0.5);
    led_three = 1;
    wait_ms(1000);
    left_wheel_enable = 1;
    right_wheel_enable = 1;
    led_two = 0;
    led_three = 0;

    while(1) {
        old_value = buffer[oldest];
        sum -= old_value;
        GyZ = gyro.getGyroRawZ();
        buffer[oldest] = GyZ;
        sum += GyZ;
        oldest++;
        if (oldest >= 1024) {
            oldest = 0;
        }
        
        if (ignore_for > 0) {
            ignore_for--;
            continue;
        }
        
        mean = sum / 1024;
        //log_mbed.putc(mean);
        if ((mean > 4) || (mean < -4)) { // threshold was 5, 2 is too low
            if ((current_command == kMoveForward) 
            || (current_command == kMoveBackward) 
            || (current_command == kMoveLongDistance)) {
                if (!GyroTurnDetected) {
                    continue_command = current_command;
                    continue_steps = steps_left;
                    if (steps_left > 2) {
                        steps_left = 2;
                    }
                }
                GyroTurnDetected = true;
            }
        }
    }
}