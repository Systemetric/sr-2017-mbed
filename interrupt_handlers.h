/* interrupt_handlers.h
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

#ifndef JDH_16_17_RASPI3_INTERRUPT_HANDLERS_H_
#define JDH_16_17_RASPI3_INTERRUPT_HANDLERS_H_

// Updates steps_gone and steps_left, and disables the PWM when at destination.
// This should be attached to the PWM interrupt.
void PwmHandler();

// Alters the current speed of the robot to perform acceleration and
// deceleration. This should be called frequently (e.g. every 0.05 seconds).
// The more often it is called, the faster the robot will change speed.
void ScaleSpeed();

// Acts upon serial commands. Should be attached to serial receive.
void SerialHandler();

#endif  // JDH_16_17_RASPI3_INTERRUPT_HANDLERS_H_