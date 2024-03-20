#pragma once
#include "libc/stdint.h"

extern void outputb(uint16_t port, uint8_t value);
extern uint8_t inputb(uint16_t port);
extern void EnableInterrupts();
extern void DisableInterrupts();

void waitio();