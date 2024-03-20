#pragma once
#include "stdint.h"
#include "stdarg.h"
#include "stdbool.h"

#define BDA_Video_Hardware 0x410                //packed bit flag to detect hardware 
#define Color_Monitor_Address 0xB8000           // most commonly used (address to print to if the video type is color)
#define Monochrome_Monitor_Address 0xB0000      // not color monitor (address to print to if the video type is monochrome)

// all the damn colors
enum Color{
    BLACK = 0x00,
    BLUE = 0x01,
    GREEN = 0x02,
    CYAN = 0x03,
    RED = 0x04,
    MAGENTA = 0x05,
    BROWN = 0x06,
    LIGHTGRAY = 0x07,
    DARKGRAY = 0x08,
    LIGHTBLUE = 0x09,
    LIGHTGREEN = 0x0A,
    LIGHTCYAN = 0x0B,
    LIGHTRED = 0x0C,
    LIGHTMAGENTA = 0x0D,
    YELLOW = 0x0E,
    WHITE = 0x0F,
    ALL = -1
};

// to check what video types are available using bios data area flags
// these value determine wheter the video type is color or monochrome
enum VideoType{
    VIDEO_TYPE_NONE = 0x00,  
    VIDEO_TYPE_COLOR = 0x20,
    VIDEO_TYPE_MONOCHROME = 0x30
};

// to get the correct video address for printing
uint16_t Check_available_video_type();
enum VideoType Get_video_type();
bool Get_correct_printing_address();


// change color of text
void Set_text_color(enum Color color);

// prints a single character to the screen
int putchar(int ic);

// prints a string to the screen by a given length
bool print(const char* data, size_t length);

// prints a full string to the screen
int printf(const char* __restrict__ format, ...);

