#pragma once
#include "libc/stdint.h"

#define empty_Port 0x80

#ifdef __cplusplus
extern "C" {
#endif

void outputb(uint16_t port, uint8_t value);
uint8_t inputb(uint16_t port);
void EnableInterrupts();
void DisableInterrupts();

#ifdef __cplusplus
}
#endif

void waitio();
