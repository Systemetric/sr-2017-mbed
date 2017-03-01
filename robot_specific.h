/* robot_specific.h
 *
 * This file is part of the code for the Hills Road/Systemetric entry to the
 * 2017 Student Robotics competition "Easy as ABC".
 *
 * Written by Josh Holland <anowlcalledjosh@gmail.com>
 *
 * This file is available under CC0. You may use it for any purpose.
 */

#ifndef JDH_16_17_RASPI3_ROBOT_SPECIFIC_H_
#define JDH_16_17_RASPI3_ROBOT_SPECIFIC_H_

#include "mbed.h"

extern const double kWheelDiameter;
extern const double kRobotDiameter;

extern const int kStepsPerRotation;
extern const int kAccelDecelSteps;
extern const int kAccelLowPowerSteps;
extern const int kInitialPwmPeriod;
extern const int kPwmPeriodIncrement;
extern const int kLowPowerThreshold;
extern DigitalOut right_wheel_direction;
extern DigitalOut left_wheel_direction;
extern DigitalOut left_wheel_enable;
extern DigitalOut right_wheel_enable;
extern AnalogIn battery_voltage;
void LeftWheelForward();
void LeftWheelBack();
void RightWheelForward();
void RightWheelBack();

extern const double kStepsPerCentimetre;
extern const double kStepsPerDegree;
extern PwmOut pwm;
extern Serial usb_serial;
extern Ticker ticker;
extern BusIn dip_switch;

#endif  // JDH_16_17_RASPI3_ROBOT_SPECIFIC_H_