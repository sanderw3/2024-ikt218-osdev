#include "Drivers/keyboardIRQ.h"
#include "Interrupts/IRQ/IRQ.h"
#include "libc/stdio.h"
#include "Interrupts/IO/IO.h"

#define KeyboardPort 0x60       // keyboard port which stores the scancode of the key pressed

// Scancodes for the keys:
// https://wiki.osdev.org/PS/2_Keyboard
// https://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html
#define UnhandeledKey '.'
#define LeftShift 0x2A
#define LeftShiftUp 0xAA
#define RightShift 0x36
#define RightShiftUp 0xB6
#define CapsLock 0x3A
#define Backspace 0x0E
#define Enter 0x1C
#define Space 0x39
#define Tab 0x0F


#define F1 0x3B
#define F2 0x3C
#define F3 0x3D
#define F4 0x3E
#define F5 0x3F
#define F6 0x40
#define F7 0x41
#define F8 0x42
#define F9 0x43
#define F10 0x44
#define F11 0x57
#define F12 0x58
#define Esc 0x01

#define Ctrl 0x1D
#define Alt 0x38

#define UpArrow 0x48
#define DownArrow 0x50
#define LeftArrow 0x4B
#define RightArrow 0x4D



// to keep track of the state of the special keys
static bool leftShift = 0;
static bool rightShift = 0;
static bool caps = 0;



// https://wiki.osdev.org/PS/2_Keyboard
// handler for the keyboard interrupt
// it reads the scancode from the keyboard and converts it to an ascii character
void keyboardIRQHandler(Interrupt_Info* info) {;
    int keystroke = inputb(KeyboardPort);   

    // for the special keys
    switch (keystroke) {
        case LeftShift: leftShift = 1; return;
        case RightShift: rightShift = 1; return;
        case LeftShiftUp: leftShift = 0; return;
        case RightShiftUp: rightShift = 0; return;
        case CapsLock: caps = !caps; return;

        // cursor movement keys:
        case UpArrow: cursorUp(); return;
        case DownArrow: cursorDown(); return;
        case LeftArrow: cursorLeft(); return;
        case RightArrow: cursorRight(); return;
        default: break;
    }


    int ascii = scancodeToAscii(keystroke);
    
    if (ascii != -1)
        putchar(ascii);
}


int scancodeToAscii(int scancode){
    // this means if the key is released
    if (scancode >= 0x81) {
        return -1;
    }
    
    // bool to check if shift is pressed
    bool shift = leftShift || rightShift;

    // bool to check if both caps and shift are pressed or either of them 
    bool upper = (caps || shift) * !(caps && shift);

    switch (scancode) {

        // normal letters
        case 0x1E: return upper == 0 ? 'a' : 'A';
        case 0x30: return upper == 0 ? 'b' : 'B';
        case 0x2E: return upper == 0 ? 'c' : 'C';
        case 0x20: return upper == 0 ? 'd' : 'D';
        case 0x12: return upper == 0 ? 'e' : 'E';
        case 0x21: return upper == 0 ? 'f' : 'F';
        case 0x22: return upper == 0 ? 'g' : 'G';
        case 0x23: return upper == 0 ? 'h' : 'H';
        case 0x17: return upper == 0 ? 'i' : 'I';
        case 0x24: return upper == 0 ? 'j' : 'J';
        case 0x25: return upper == 0 ? 'k' : 'K';
        case 0x26: return upper == 0 ? 'l' : 'L';
        case 0x32: return upper == 0 ? 'm' : 'M';
        case 0x31: return upper == 0 ? 'n' : 'N';
        case 0x18: return upper == 0 ? 'o' : 'O';
        case 0x19: return upper == 0 ? 'p' : 'P';
        case 0x10: return upper == 0 ? 'q' : 'Q';
        case 0x13: return upper == 0 ? 'r' : 'R';
        case 0x1F: return upper == 0 ? 's' : 'S';
        case 0x14: return upper == 0 ? 't' : 'T';
        case 0x16: return upper == 0 ? 'u' : 'U';
        case 0x2F: return upper == 0 ? 'v' : 'V';
        case 0x11: return upper == 0 ? 'w' : 'W';
        case 0x2D: return upper == 0 ? 'x' : 'X';
        case 0x15: return upper == 0 ? 'y' : 'Y';
        case 0x2C: return upper == 0 ? 'z' : 'Z';

        // this is for the numbers
        case 0x02: return shift == 1 ? '!' : '1';
        case 0x03: return shift == 1 ? '@' : '2';
        case 0x04: return shift == 1 ? '#' : '3';
        case 0x05: return shift == 1 ? '$' : '4';
        case 0x06: return shift == 1 ? '%' : '5';
        case 0x07: return shift == 1 ? '^' : '6';
        case 0x08: return shift == 1 ? '&' : '7';
        case 0x09: return shift == 1 ? '*' : '8';
        case 0x0A: return shift == 1 ? '(' : '9';
        case 0x0B: return shift == 1 ? ')' : '0';

        // this is for the special characters
        case 0x0C: return shift == 1 ? '_' : '-';   
        case 0x0D: return shift == 1 ? '+' : '=';
        case 0x1A: return shift == 1 ? '{' : '[';
        case 0x1B: return shift == 1 ? '}' : ']';
        case 0x27: return shift == 1 ? ':' : ';';
        case 0x28: return shift == 1 ? '"' : '\'';
        case 0x29: return shift == 1 ? '~' : '`';
        case 0x33: return shift == 1 ? '<' : ',';
        case 0x34: return shift == 1 ? '>' : '.';
        case 0x35: return shift == 1 ? '?' : '/';
        case 0x2B: return shift == 1 ? '|' : '\\';

        // action keys
        case Enter: return '\n';
        case Space: return ' ';
        case Tab: return '\t';
        case Backspace: return '\b';
        default: return UnhandeledKey;
    }
}

