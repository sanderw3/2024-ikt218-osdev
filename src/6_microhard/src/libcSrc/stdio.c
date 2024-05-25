#include "libc/stdio.h"
#include "libc/stdarg.h"
#include "libc/stdint.h"
#include "libc/stdarg.h"
#include "libc/stdbool.h"
#include "libc/stddef.h"
#include "libc/string.h"
#include "Drivers/display.h"


//screen offsets used when printing (for the cursor)
static int y = 0;
static int x = 0;
int CurrentCol = 0;

// for printing with precision
static short precision = 5;


//configurations
static enum Color Text_color = LIGHTGRAY;                       // default text color   
static enum VideoType Current_Video_type = VIDEO_TYPE_NONE;     // current video type is first undetecte, will be detected once putchar is called.
static uint8_t* Video_memory = NULL;                            // Video_memory is the location to where we will directly enter our strings


bool Get_correct_printing_address(){
    // first time running we need to set the video type, so we can print to the proper screen address
    if (Current_Video_type == VIDEO_TYPE_NONE) Current_Video_type = Get_video_type();

    // Now we get the correct address to print to based on the video type
    if (Video_memory == NULL){
        switch (Current_Video_type){
        case VIDEO_TYPE_COLOR: Video_memory = (uint8_t*) Color_Monitor_Address; break;              // Color monitor address (0xB8000)
        case VIDEO_TYPE_MONOCHROME: Video_memory = (uint8_t*) Monochrome_Monitor_Address; break;    // Monochrome monitor address (0xB0000)
        default: return false;
        }
    }
    return true;
}

// for fun changes the color of the text printed
void Set_text_color(enum Color color){
    Text_color = color;
}


int printf(const char* __restrict__ format, ...){
    va_list arguments;
    va_start(arguments, format);
    
    //since print normally returns the number of characters printd, we have to count the number of characters printed
    int count = 0;


    while(*format){
        if (*format == '%'){
            format++; // skipping the %
        
            switch (*format) {

            //for single characters
            case 'c':{ 
                format++; // skipping the c

                //prints character to screen
                putchar(va_arg(arguments, int));
                count++;
                break;
            }

            //for strings
            case 's':{
                format++;

                //get string from parameter
                char *_innerString = va_arg(arguments, char*);
                
                //print until the strings ends
                while (*_innerString){ 
                        putchar(*_innerString++); 
                        count++;
                    }
                break;
            }
            
            //for integers
            case 'd':
            case 'i':{
                format++;
                char _innerBuffer[11]; // 11 is enough for an int in a 32 bit system as it never exceeds "-2147483646" in length

                //convert integer to string
                char *_innerIntAsString = itoa(va_arg(arguments, int), _innerBuffer, 10);

                // print string
                while (*_innerIntAsString){
                    putchar(*_innerIntAsString++);
                    count++;
                }

                break;
            }
            
            //for floats
            // here we don't have format++; cus its done in the while loop    
            case '.':{           
                // creating temporary variable to store precision
                char _innerPrecision[2];
                short i = 0;
                //while loop is to get precision (all numbers the user may enter, even 10000, but only 2 will be used). This will remove all other numbers
                while (isInt(*(++format))){
                    // only gets the two first
                    if (i < 2){
                        // stores the given precision until its full
                        _innerPrecision[i++] = *format;
                    }
                }
                precision = atoi(_innerPrecision);
            }

            //for floats IEEE 754 for this part
            case 'f':{
                format++;
                char _innerBuffer[40]; // since floats can produce longer numbers than integers we use 40

                //get float from parameter
                double _innerFloat = va_arg(arguments, double);

                //convert float to string
                char *_innerFloatAsString = dtoa(_innerFloat, _innerBuffer, precision);

                //print the string
                while (*_innerFloatAsString){
                    putchar(*_innerFloatAsString++);
                    count++;
                }
                break;
            }
            
            case 'x':
            case 'X':{
                format++;
                // get hex value as unsigned int
                unsigned int _innerHex = va_arg(arguments, unsigned int);
                
                // buffer to store the entered hex value
                char _innerBuffer[20];
                
                // convert integer value to hex (string format)
                char* _innerHexAsString = itoh(_innerHex, _innerBuffer);
                while (*_innerHexAsString){
                    putchar(*_innerHexAsString++);
                    count++;
                }
            }
            default:
                break;
            }

        }

        // writes the character from the original string
        putchar(*format++);
        count++;
    }

    va_end(arguments);
    return count;
}


bool print(const char* data, size_t length){
    if (length <= 0) return false;

    // call putchar for each char in the string, base on the length
    for (size_t i = 0; i < length; i++){
        if(putchar(data[i]) == -1)
            return false;
    }
    return true;
}


int putchar(int ic){
    //first we have to get correct addresses to write to:
    if( Video_memory == NULL || Current_Video_type == VIDEO_TYPE_NONE ) {
        // if we can't get the correct address to print to, return -1 as error
        if (Get_correct_printing_address() == false) return -1; 
    }

    switch (ic) {
    case '\n':                          // check if we have to print a new line
        x = 0;                          //back left side of screen
        y++;                            // jump down one row
        updateCursor();
        return ic;
    
    case '\b':                          // check if we have a backspace    
        if (x > 0){                     // if \b is at anywhere but the beginning of the line, do as normal:
            x--;                        // move back one character
            putchar('\0');               // print nothing to overwrite the character
            x--;                        // move back one character             
        } else if (y > 0){
            x = ScreenWidth - 1;        // if we are at the beginning of the line, we go back to the previous line
            y--;                        // move up one row
        }
        updateCursor();
        return ic;

    case '\t':                          // check if we have a tab
        x += 4;                         // move 4 characters to the right
        updateCursor();
        return ic;

    default:
        if (x >= ScreenWidth) {
            x = 0;
            y++;
        }
        break;
    }

    
    // scroll if needed
    updateView();


    enum Color mycolor = Text_color;

    // this is just to make all the characters switch between different colors (for fun)
    if (Text_color == ALL){ // currentCol is incremented for each character, but if it exceeds 0x0F we start over
        mycolor = (CurrentCol++ - (0x0F * (CurrentCol > 0x0F))) % 0x10;
    }


    // manually keeping track of the cursor position
    // now get the propriate offsett based on already printet characters.
    // 80 = size of screen width. 
    // We multiply by 2 since offset + 1 will change the color, so we have to skip the color for each new character
    int offset = (y * ScreenWidth + x++) * 2;     

    // write the character
    Video_memory[offset] = ic;
    // set the color
    Video_memory[offset + 1] = mycolor;

    moveCursor(y, x);
    return ic;
}


// cursor functions
updateCursor(){
    moveCursor(y, x);
}
void cursorRight(){
    if (x < ScreenWidth)        {x++; }                                 // moves right
    else if (y < ScreenHeight)  {y++; x=0; }                            // moves down
    else                        {y = 0; x = 0; }                        // loops back to the top
    moveCursor(y, x);
}
void cursorLeft(){
    if (x > 0)                  {x--;}
    else if (y > 0)             {y--; x = ScreenWidth;}                 // if we are at the beginning of the line, we go back to the previous line
    else                        {y = ScreenHeight; x = ScreenWidth;}    // loops back to the bottom
    moveCursor(y, x);
}
void cursorUp(){
    if (y > 0)                  {y--;}
    else                        {y = ScreenHeight;}                     // loops back to the top
    moveCursor(y, x);
}
void cursorDown(){
    if (y < ScreenHeight)       {y++;}
    else                        {y = 0;}                                // loops back to the bottom
    moveCursor(y, x);
}




updateView(){
    // scrolling function:
    if (y >= ScreenHeight){
        // if we are at the end of the screen, we scroll
        for (unsigned short i = 0; i < ScreenHeight; i++){
            for (unsigned short column = 0; column < ScreenWidth; column++){

                // find the row of the next line, based on the screenwidth
                unsigned short nextRow = (i + 1) * ScreenWidth;
                // the row of the current line
                unsigned short currentRow = i * ScreenWidth;
                
                // copy the next line to the current 
                Video_memory[(currentRow + column) * 2]     = Video_memory[(nextRow + column) * 2];
                Video_memory[(currentRow + column) * 2 + 1] = Video_memory[(nextRow + column) * 2 + 1];
            }
        }

        // fill the last line with spaces
        for (unsigned short column = 0; column < ScreenWidth; column++){
            Video_memory[((ScreenHeight-1)*ScreenWidth + column) * 2] = ' ';                // empty space
            Video_memory[((ScreenHeight-1)*ScreenWidth + column) * 2 + 1] = LIGHTGRAY;      // default color
        }

        // reset the cursor
        y = ScreenHeight - 1;
        x = 0;
    }
    // end of scrolling
}