#pragma once
#include "Interrupts/ISR/ISR.h"
#include "Interrupts/IO/IO.h"


// based off of https://wiki.osdev.org/Detecting_Colour_and_Monochrome_Monitors
// and https://wiki.osdev.org/Printing_To_Screen
#define BDA_Video_Hardware 0x410                //packed bit flag to detect hardware 
#define Color_Monitor_Address 0xB8000           // most commonly used (address to print to if the video type is color)
#define Monochrome_Monitor_Address 0xB0000      // not color monitor (address to print to if the video type is monochrome)
#define VGA_Control_Register 0x3D4              // VGA control register
#define VGA_Data_Register 0x3D5                 // VGA data register

// default screen size
#define ScreenWidth 80
#define ScreenHeight 25



void moveCursor(int x, int y);                   // to set the cursor a specific x and y position
void setCursorShape(uint8_t start, uint8_t end); // to set the cursor shape


// to check what video types are available using bios data area flags
uint16_t Check_available_video_type();
enum VideoType Get_video_type();


// to check what video types are available using bios data area flags
// these value determine wheter the video type is color or monochrome
enum VideoType{
    VIDEO_TYPE_NONE = 0x00,  
    VIDEO_TYPE_COLOR = 0x20,
    VIDEO_TYPE_MONOCHROME = 0x30
};


//examples:
void setBlockCursor() ;

// Example 2: Underline Cursor (appears as a line at the bottom of the cell)
void setUnderlineCursor() ;

// Example 3: Thin Horizontal Line Cursor (appears as a thin line in the middle of the cell)
void setThinLineCursor() ;

// Example 4: Half Block Cursor (covers the upper half of the character cell)
void setHalfBlockCursor() ;
