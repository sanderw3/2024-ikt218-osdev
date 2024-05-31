#ifndef Command_H
#define Command_H

#include <stdint.h>

constexpr uint32_t fnv1a_hash(const char *str, uint32_t hash);
void performCommand(const char* command);
void printHelp();
void playSong();

#endif