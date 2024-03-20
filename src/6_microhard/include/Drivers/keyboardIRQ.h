#pragma once
#include "Interrupts/ISR/ISR.h"
#include "Interrupts/IO/IO.h"

int scancodeToAscii(int scancode);

void keyboardIRQHandler(Interrupt_Info* info);
