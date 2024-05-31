#include "Drivers/display.h"

// Function to set the cursor shape
void setCursorShape(uint8_t start, uint8_t end) {
    // Send the start scanline of the cursor
    outputb(VGA_Control_Register, 0x0A);
    outputb(VGA_Data_Register, (inputb(VGA_Data_Register) & 0xC0) | start);

    // Send the end scanline of the cursor
    outputb(VGA_Control_Register, 0x0B);
    outputb(VGA_Data_Register, (inputb(VGA_Data_Register) & 0xE0) | end);
}

void moveCursor(int row, int col) {
    uint16_t pos = (row * ScreenWidth) + col;  

    // Send the high cursor byte
    outputb(VGA_Control_Register, 0x0E);
    outputb(VGA_Data_Register, (uint8_t)((pos >> 8) & 0xFF));

    // Send the low cursor byte
    outputb(VGA_Control_Register, 0x0F);
    outputb(VGA_Data_Register, (uint8_t)(pos & 0xFF));
}


// FROM https://wiki.osdev.org/Detecting_Colour_and_Monochrome_Monitors
// to get available video hardware
uint16_t Check_available_video_type(){
    return *((const uint16_t*) BDA_Video_Hardware);
}

// to get video type from available video hardware
enum VideoType Get_video_type(){
    return (enum VideoType) (Check_available_video_type() & VIDEO_TYPE_COLOR);
}


//examples

// Example 1: Block Cursor (covers the entire character cell)
void setBlockCursor() {
    setCursorShape(0, 15);  // Start at scanline 0, end at scanline 15
}

// Example 2: Underline Cursor (appears as a line at the bottom of the cell)
void setUnderlineCursor() {
    setCursorShape(13, 14);  // Start at scanline 13, end at scanline 14
}

// Example 3: Thin Horizontal Line Cursor (appears as a thin line in the middle of the cell)
void setThinLineCursor() {
    setCursorShape(7, 8);  // Start at scanline 7, end at scanline 8
}

// Example 4: Half Block Cursor (covers the upper half of the character cell)
void setHalfBlockCursor() {
    setCursorShape(0, 7);  // Start at scanline 0, end at scanline 7
}
