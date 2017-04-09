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
extern bool GyroTurnDetected;
extern volatile int steps_left;
extern volatile int continue_steps;
extern int ignore_for;
enum CurrentCommand {
    kNone,
    kMoveForward,
    kMoveBackward,
    kMoveLongDistance,
    kTurnLeft,
    kTurnRight
};
extern volatile CurrentCommand current_command;
extern volatile CurrentCommand continue_command;

#endif  // JDH_16_17_RASPI3_INTERRUPT_HANDLERS_H_