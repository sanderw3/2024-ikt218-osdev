#pragma once
#include "Interrupts/ISR/ISR.h"
#include "Interrupts/IO/IO.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

int scancodeToAscii(int scancode);

void keyboardIRQHandler(Interrupt_Info* info);

// to keep track of if the has been used yet
bool keyboardUsed();
bool keyboardNotFinished();

void setKeyboardNotUsed();


#ifdef __cplusplus
}
#endif