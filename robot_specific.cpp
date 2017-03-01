/* robot_specific.cpp
 *
 * This file is part of the code for the Hills Road/Systemetric entry to the
 * 2017 Student Robotics competition "Easy as ABC".
 *
 * Written by Josh Holland <anowlcalledjosh@gmail.com>
 *
 * This file is available under CC0. You may use it for any purpose.
 */

#include "robot_specific.h"

#include "mbed.h"

/****************** These should be updated for each robot. *******************/

// The diameter of the wheels, in centimetres.
const double kWheelDiameter = 13.36;
// Prototype = 13.36

// The distance between the middle of the wheels, in centimetres. Increasing
// this number will decrease the number of steps the robot takes per degree.
const double kRobotDiameter = 40.30;  // 42.79 * 17/18 * 359/360
// Prototype = 42.79

/************** These will probably be OK to leave as they are. ***************/

// The number of steps the motors must take to rotate 360 degrees.
const int kStepsPerRotation = 3200;

// The number of steps to accelerate and decelerate for.
const int kAccelDecelSteps = 6000;

// The number of steps to accelerate for when in the low-power state.
const int kAccelLowPowerSteps = 2000;

// The initial PWM period, in microseconds.
const int kInitialPwmPeriod = 1000;

// How much to change the PWM period by at a time, in microseconds.
const int kPwmPeriodIncrement = 10;

// The voltage at the motor boards, as a fraction, at which the robot should
// slow down to minimum speed to decrease the likelihood of the motors stalling.
const int kLowPowerThreshold = 0.6;

// The pins that the motor control boards' DIR pins are connected to.
DigitalOut right_wheel_direction(p16);
DigitalOut left_wheel_direction(p17);

DigitalOut left_wheel_enable(p21);
DigitalOut right_wheel_enable(p22);

AnalogIn battery_voltage(p15);

// What to set the DIR pins to in order to travel in a certain direction.
// (Unfortunately these can't be declared inline, since they're defined in a
// different compilation unit to where they're used -- on the other hand, they
// aren't called that often.)
void LeftWheelForward() {left_wheel_direction = 0;}
void LeftWheelBack() {left_wheel_direction = 1;}
void RightWheelForward() {right_wheel_direction = 1;}
void RightWheelBack() {right_wheel_direction = 0;}

/******************** These shouldn't usually be changed. *********************/

// This isn't defined in the mbed's math.h, so we define it here.
const double kPi = 3.14159265358979323846;

const double kWheelCircumference = kWheelDiameter * kPi;
const double kRobotCircumference = kRobotDiameter * kPi;

// The number of steps to take to move one centimetre (probably about 80).
const double kStepsPerCentimetre = kStepsPerRotation / kWheelCircumference;
// The number of steps to take to rotate one degree (probably about 34).
const double kStepsPerDegree = kRobotCircumference / kWheelCircumference
                               * kStepsPerRotation / 360;

// The pin that *both* motor control boards' CLK pins are connected to.
PwmOut pwm(p23);

// The USB serial connection.
Serial usb_serial(USBTX, USBRX);

// Responsible for calling ScaleSpeed at regular intervals.
Ticker ticker;

BusIn dip_switch(p14);