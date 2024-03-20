#pragma once
#include "libc/stdint.h"
#include "libc/stdbool.h"


//returns the length of a string
size_t strlen(const char* str);

// for integers
// converts a number to a string (this particular function has only been made to base 10 for now)
char* itoa(int num, char* buf);
//converts a string to an int
int atoi(const char* str);


// for floats
// converts a float value to a string
char* dtoa(double num, char* buf, int precision);

// to check if an ascii character is a digit
bool isInt(char c);


// double changePrecision(double number, int precision);
