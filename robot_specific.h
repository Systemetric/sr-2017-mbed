/* robot_specific.h
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

#ifndef JDH_16_17_RASPI3_ROBOT_SPECIFIC_H_
#define JDH_16_17_RASPI3_ROBOT_SPECIFIC_H_

#include "mbed.h"

// These are robot-specific constants that should be changed for every robot.
// If this is not done, the robot will not move accurately.

// The number of steps the motor must take to rotate 360 degrees.
extern const int kStepsPerRotation;

// The diameter of the wheels, in centimetres.
extern const double kWheelDiameter;

// The distance between the wheels, in centimetres.
extern const double kRobotDiameter;

// The initial PWM period, in microseconds.
extern const int kInitialPwmPeriod;

// The number of steps to accelerate and decelerate for.
extern const int kAccelDecelSteps;

// How much to change the PWM period by at a time, in microseconds.
extern const int kPwmPeriodIncrement;

// The number of steps to take to move one centimetre.
extern const double kStepsPerCentimetre;

// The number of steps to take to rotate one degree.
extern const double kStepsPerDegree;

extern DigitalOut right_wheel_direction;
extern DigitalOut left_wheel_direction;

void LeftWheelForward();
void LeftWheelBack();
void RightWheelForward();
void RightWheelBack();

extern PwmOut pwm;
extern Serial usb_serial;
extern Ticker ticker;

#endif  // JDH_16_17_RASPI3_ROBOT_SPECIFIC_H_