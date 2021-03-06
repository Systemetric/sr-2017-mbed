/* interrupt_handlers.cpp
 *
 * This file is part of the code for the Hills Road/Systemetric entry to the
 * 2017 Student Robotics competition "Easy as ABC".
 *
 * Written by Josh Holland <anowlcalledjosh@gmail.com>
 *
 * This file is available under CC0. You may use it for any purpose.
 */
 
 // Bear in mind that ISRs -- interrupt service routines -- are not
 // interruptible, so any interrupts generated whilst execution is inside one of
 // these functions will be silently discarded. Therefore, they should all
 // execute as quickly as possible; things like reading from the serial
 // connection are extremely slow, even at 115200 baud.

#include "interrupt_handlers.h"

#include "mbed.h"

#include "robot_specific.h"
#include "pwm_constants.h"

#define DECEL_FACTOR 4

enum State {
    kReadyForCommand,
    // k*Received means "waiting for argument(s)", i.e. how far to move
    kForwardReceived,
    kBackwardReceived,
    kLongDistanceReceived,
    kLeftReceived,
    kRightReceived
};

volatile int steps_gone = 0;
extern volatile int steps_left = 0;
extern volatile int continue_steps = 0;
volatile int pwm_period = 0;
volatile char dip_switch_state = 0;
volatile State robot_state = kReadyForCommand;
volatile bool period_update_needed = false;
volatile bool force_low_power = false;
// Track the currently executing command so that we can respond with its letter
// when it ends.
extern volatile CurrentCommand current_command = kNone;
extern volatile CurrentCommand continue_command = kNone;

// DigitalOuts are inherently "volatile", so they don't need to be explicitly
// declared as such. If they are, things break.
DigitalOut led_pwm_interrupt(p11);
DigitalOut led_accelerate(p12);
DigitalOut led_decelerate(p13);
DigitalOut led_left(p14);
DigitalOut led_right(p10);
DigitalOut low_power(LED1);
DigitalOut updating_speed(LED4);


// This keeps track of how far the robot has gone, and stops it when it's gone
// far enough. It also sends a character over serial to say that it's finished.
void PwmHandler() {
    // Check whether a match 4 interrupt is pending (we're only interested if it is).
    if (LPC_PWM1->IR & PWM_IR_MR4) {
        led_pwm_interrupt = !led_pwm_interrupt;
        steps_gone++;
        steps_left--;
        if (steps_left <= 0) {            
            pwm.pulsewidth_ms(0);  // disable PWM by setting duty cycle to 0
            //usb_serial.putc('d');  // say we're done driving
            switch (current_command) {
                case kMoveForward:
                    if (!GyroTurnDetected) {
                        usb_serial.putc('f');
                    } else {
                        usb_serial.putc('e');
                    }
                    break;
                case kMoveBackward:
                    if (!GyroTurnDetected) {
                        usb_serial.putc('b');
                    } else {
                        usb_serial.putc('e');
                    }
                    break;
                case kMoveLongDistance:
                    if (!GyroTurnDetected) {
                        usb_serial.putc('F');
                    } else {
                        usb_serial.putc('e');
                    }
                    break;
                case kTurnLeft:
                    usb_serial.putc('l');
                    break;
                case kTurnRight:
                    usb_serial.putc('r');
                    break;
                default:
                    //usb_serial.putc('d');
                    break;
            }
            current_command = kNone;
        }
        LPC_PWM1->IR = PWM_IR_MR4;  // clear the interrupt flag (yes, by writing a 1 to it)
    }
    if (LPC_PWM1->IR & PWM_IR_MR0) {
        // Update speed calculated by ScaleSpeed
        if (period_update_needed) {
            pwm.period_us(pwm_period);
            pwm = 0.5;
            period_update_needed = false;
        }
        LPC_PWM1->IR = PWM_IR_MR0;
    }
    // Clear all interrupts, just in case one we aren't interested in fired.
    //LPC_PWM1->IR = PWM_IR_ALL;
}

// Alters the current speed of the robot to perform acceleration and
// deceleration. The more often this is called, the faster the robot will change
// speed. In order to disable acceleration, comment out the ticker.attach(...)
// line in main.cpp.
void ScaleSpeed() {
    if (steps_left == 0) {
        led_accelerate = 0;
        led_decelerate = 0;
        return;  // No point scaling speed when not moving.
    }

    period_update_needed = true; // probably
    if (steps_gone < kAccelDecelSteps) {
        // not gone far, probably accelerate
        if (steps_left < (steps_gone / DECEL_FACTOR)) {
            // more than halfway there (but still close to start), decelerate
            if (pwm_period < kInitialPwmPeriod) {
                led_accelerate = 0;
                led_decelerate = 1;
                pwm_period += (kPwmPeriodIncrement * DECEL_FACTOR);
            }
        } else {
            if (battery_voltage.read() < kLowPowerThreshold || force_low_power) {
                // Low power
                if (steps_gone < kAccelLowPowerSteps) {
                    // accelerate anyway
                    led_accelerate = 1;
                    led_decelerate = 0;
                    pwm_period -= kPwmPeriodIncrement;
                }
            } else {
                // less than halfway there (and not gone far), accelerate
                led_accelerate = 1;
                led_decelerate = 0;
                pwm_period -= kPwmPeriodIncrement;
            }
        }
    } else if (steps_left < (kAccelDecelSteps / DECEL_FACTOR)) {
        if (pwm_period < kInitialPwmPeriod) {
            // almost done moving, decelerate
            led_accelerate = 0;
            led_decelerate = 1;
            pwm_period += (kPwmPeriodIncrement * DECEL_FACTOR);
        }
    } else {
        // In the middle of movement, don't change speed.
        led_accelerate = 1;
        led_decelerate = 1;
        period_update_needed = false;
    }
    //pwm.period_us(pwm_period);
    // Make sure there is both a rising and falling edge at some point in the
    // cycle.
    //pwm = 0.5;
}

// This is called every time the mbed receives a character over serial. It sets
// the wheels to turn in the right direction, and makes the robot start moving.
void SerialHandler() {
    if (!usb_serial.readable()) {
        // There's not actually anything available to read(!), abort.
        return;
    }
    char character = usb_serial.getc();
    switch (robot_state) {
        case kReadyForCommand:
            GyroTurnDetected = false;
            ignore_for = 1024;
            switch (character) {
                case 'f':  // move forwards
                    force_low_power = false;
                    robot_state = kForwardReceived;
                    break;
                case 'b':  // move backwards
                    robot_state = kBackwardReceived;
                    break;
                case 'F':  // move forwards lots
                    robot_state = kLongDistanceReceived;
                    break;
                case 'l':  // turn left
                    led_left = 1;
                    led_right = 0;
                    robot_state = kLeftReceived;
                    break;
                case 'r':  // turn right
                    led_right = 1;
                    led_left = 0;
                    robot_state = kRightReceived;
                    break;
                case 's':  // get DIP switch state
                    dip_switch_state = (char)dip_switch.read();
                    usb_serial.putc(dip_switch_state);
                    break;
                case 'c': // continue after a collision with whatever was being done
                    // The robot might judder slightly if called if a crash hasn't occured first
                    // Either that or move forwards uncontrollably.
                    current_command = continue_command;
                    // Stepper motors already configured correctly
                    steps_left = continue_steps;
                    steps_gone = 0;
                    pwm_period = kInitialPwmPeriod;
                    pwm.period_us(pwm_period);
                    pwm = 0.5;
                    robot_state = kReadyForCommand;
                    break;
                case 'A':
                    force_low_power = true;
                    robot_state = kForwardReceived;
                    break;
            }
            break;
        case kForwardReceived:
            current_command = kMoveForward;
            steps_left = character * kStepsPerCentimetre;
            steps_gone = 0;
            LeftWheelForward();
            RightWheelForward();
            pwm_period = kInitialPwmPeriod;
            pwm.period_us(pwm_period);
            pwm = 0.5;
            robot_state = kReadyForCommand;
            break;
        case kBackwardReceived:
            current_command = kMoveBackward;
            steps_left = character * kStepsPerCentimetre;
            steps_gone = 0;
            LeftWheelBack();
            RightWheelBack();
            pwm_period = kInitialPwmPeriod;
            pwm.period_us(pwm_period);
            pwm = 0.5;
            robot_state = kReadyForCommand;
            break;
        case kLongDistanceReceived:
            current_command = kMoveLongDistance;
            steps_left = character * 10 * kStepsPerCentimetre;
            steps_gone = 0;
            LeftWheelForward();
            RightWheelForward();
            pwm_period = kInitialPwmPeriod;
            pwm.period_us(pwm_period);
            pwm = 0.5;
            robot_state = kReadyForCommand;
            break;
        case kLeftReceived:
            current_command = kTurnLeft;
            steps_left = character * kStepsPerDegree;
            steps_gone = 0;
            LeftWheelBack();
            RightWheelForward();
            pwm_period = kInitialPwmPeriod;
            pwm.period_us(pwm_period);
            pwm = 0.5;
            robot_state = kReadyForCommand;
            break;
        case kRightReceived:
            current_command = kTurnRight;
            steps_left = character * kStepsPerDegree;
            steps_gone = 0;
            LeftWheelForward();
            RightWheelBack();
            pwm_period = kInitialPwmPeriod;
            pwm.period_us(pwm_period);
            pwm = 0.5;
            robot_state = kReadyForCommand;
            break;
    }
}