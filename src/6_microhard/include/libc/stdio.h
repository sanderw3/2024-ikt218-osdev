#pragma once
#include "stdint.h"
#include "stdarg.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

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
    DEFAULT = 0x07,
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

// to get the correct video address for printing
bool Get_correct_printing_address();

// change color of text
void Set_text_color(enum Color color);

// prints a single character to the screen
int putchar(int ic);

// prints a string to the screen by a given length
bool print(const char* data, size_t length);

// prints a full string to the screen
int printf(const char* __restrict__ format, ...);


// extra stuff which is not needed for stdio:
// scrolls the screen if needed
void updateView();

// clears the screen
void clearScreen();

// prints to the screen with a delay
void dramaticPrint(const char* string, uint32_t delay);

//to move the cursors position:
void updateCursor();                      // to update the cursor position to the current x and y 
void cursorRight();                       // for the right arrow key
void cursorLeft();                        // for the left arrow key
void cursorUp();                          // for the up arrow key
void cursorDown();                        // for the down arrow key

void executeCommand();
extern void performCommand(const char* command);
void printSplashScreen();
void moveToBeginningOfLine();


#ifdef __cplusplus
}
#endif