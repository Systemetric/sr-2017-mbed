/* interrupt_handlers.h
 *
 * This file is part of the code for the Hills Road/Systemetric entry to the
 * 2017 Student Robotics competition "Easy as ABC".
 *
 * Written by Josh Holland <anowlcalledjosh@gmail.com>
 *
 * This file is available under CC0. You may use it for any purpose.
 */

#ifndef JDH_16_17_RASPI3_INTERRUPT_HANDLERS_H_
#define JDH_16_17_RASPI3_INTERRUPT_HANDLERS_H_

void PwmHandler();
void ScaleSpeed();
void SerialHandler();

#endif  // JDH_16_17_RASPI3_INTERRUPT_HANDLERS_H_