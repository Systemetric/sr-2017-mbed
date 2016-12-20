/* robot_specific.cpp
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

// These should be changed for each robot.
const double kWheelDiameter = 12.6;
const double kRobotDiameter = 43.5;
// These will probably be OK to leave as they are, but may need changing.
const int kStepsPerRotation = 3200;
const int kInitialPwmPeriod = 1000;
const int kAccelDecelSteps = 8000;  // maybe reduce this? too much speed...
const int kPwmPeriodIncrement = 10;
DigitalOut right_wheel_direction(p16);
DigitalOut left_wheel_direction(p17);
void LeftWheelForward() {left_wheel_direction = 1;}
void LeftWheelBack() {left_wheel_direction = 0;}
void RightWheelForward() {right_wheel_direction = 0;}
void RightWheelBack() {right_wheel_direction = 1;}

// This isn't defined in the mbed's math.h, so we define it here.
const double kPi = 3.14159265358979323846;

const double kWheelCircumference = kWheelDiameter * kPi;
const double kRobotCircumference = kRobotDiameter * kPi;

// For the 2016-17 stepper testbed, this is approximately 80.
const double kStepsPerCentimetre = kStepsPerRotation / kWheelCircumference;
// For the 2016-17 stepper testbed, this is approximately 34.
const double kStepsPerDegree = kRobotCircumference / kWheelCircumference
                               * kStepsPerRotation / 360;

// robotc * steps_per_r
// --------------------
//     wheelc * 360

PwmOut pwm(p23);
Serial usb_serial(USBTX, USBRX);
Ticker ticker;