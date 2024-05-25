#pragma once
#include <stdint.h>
#include "Interrupts/ISR/ISR.h"

uint32_t millis();

void timerIRQHandler(Interrupt_Info* info);
