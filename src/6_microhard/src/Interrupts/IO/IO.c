#include "Interrupts/IO/IO.h"


// this function is used to output 0 to an unused port (0x80)
// this is mostly for systems where the pic is not integrated into the cpu, as it isn't the fastest chip
// this is used to wait for the pic to be ready
void waitio() {
    outputb(empty_Port, 0x00);
}