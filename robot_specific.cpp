/* robot_specific.cpp
 *
 * This file contains robot-specific constants that should be updated for every
 * robot. If this is not done, the robot will not move accurately.
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

#include "robot_specific.h"

#include "mbed.h"

/****************** These should be updated for each robot. *******************/

// The diameter of the wheels, in centimetres.
const double kWheelDiameter = 13.36;

// The distance between the middle of the wheels, in centimetres.
const double kRobotDiameter = 42.79;

/************** These will probably be OK to leave as they are. ***************/

// The number of steps the motors must take to rotate 360 degrees.
const int kStepsPerRotation = 3200;

// The number of steps to accelerate and decelerate for.
const int kAccelDecelSteps = 8000;

// The initial PWM period, in microseconds.
const int kInitialPwmPeriod = 1000;

// How much to change the PWM period by at a time, in microseconds.
const int kPwmPeriodIncrement = 10;

// The pins that the motor control boards' DIR pins are connected to.
DigitalOut right_wheel_direction(p16);
DigitalOut left_wheel_direction(p17);

// What to set the DIR pins to in order to travel in a certain direction.
// (Unfortunately these can't be declared inline, since they're defined in a
// different compilation unit to where they're used -- on the other hand, they
// aren't called that often.)
void LeftWheelForward() {left_wheel_direction = 1;}
void LeftWheelBack() {left_wheel_direction = 0;}
void RightWheelForward() {right_wheel_direction = 0;}
void RightWheelBack() {right_wheel_direction = 1;}

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