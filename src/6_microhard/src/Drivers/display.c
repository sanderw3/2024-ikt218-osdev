#include "Drivers/display.h"

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
