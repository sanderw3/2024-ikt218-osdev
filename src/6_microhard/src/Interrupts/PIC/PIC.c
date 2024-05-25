#include "Interrupts/PIC/PIC.h"
#include "Interrupts/IO/IO.h"
#include "libc/stdint.h"

void initialize_pic(uint8_t master_offset, uint8_t slave_offset) {

    uint8_t a1, a2;
 
	a1 = inputb(MASTER_PIC_DATA_PORT);                        // save masks
	a2 = inputb(SLAVE_PIC_DATA_PORT);

// control word 1
    // Initialize the master PIC
    outputb(MASTER_PIC_COMMAND_PORT,
        PIC_INIT(1) | PIC_LEVEL(0) | PIC_INTERVAL4(0) | PIC_SINGLE(0) | PIC_ICW4(1) // equivalent to 0001 0001 (0x11)
    );
    waitio(); 

    // Initialize the slave PIC
    outputb(SLAVE_PIC_COMMAND_PORT,
        PIC_INIT(1) | PIC_LEVEL(0) | PIC_INTERVAL4(0) | PIC_SINGLE(0) | PIC_ICW4(1) // equivalent to 0001 0001 (0x11)
    ); 
    waitio();    

// control word 2
    // Set the offsets
    outputb(MASTER_PIC_DATA_PORT, master_offset);
    waitio();
    outputb(SLAVE_PIC_DATA_PORT, slave_offset);
    waitio();

// control word 3
    // Tell the master PIC that there is a slave PIC at IRQ2 (0000 0100)
    outputb(MASTER_PIC_DATA_PORT, 0x04);
    waitio();
    // Tell the slave PIC its cascade identity (0000 0010)
    outputb(SLAVE_PIC_DATA_PORT, 0x02);
    waitio();

// control word 4
    // Set the mode
    outputb(MASTER_PIC_DATA_PORT, PIC_8086(1));
    waitio();
    outputb(SLAVE_PIC_DATA_PORT, PIC_8086(1));
    waitio();

// clear the data port
    // Clear the data port
    outputb(MASTER_PIC_DATA_PORT, a1);
    waitio();
    outputb(SLAVE_PIC_DATA_PORT, a2);
    waitio();

}




// what is this?>?!?!?
///////////////////////////////////////////////////////////////////////////////////////////////////

// non specific end of interrupt
void end_of_interrupt(int irqNum) {
    if (irqNum >= 8) {
        outputb(SLAVE_PIC_COMMAND_PORT, PIC_EOI);
    }
    outputb(MASTER_PIC_COMMAND_PORT, PIC_EOI);
}


// masks the pic (disables the irq)
void mask_pic(int irqNum) {
    if (irqNum < 8) {
        uint8_t mask = inputb(MASTER_PIC_DATA_PORT);
        mask |= (1 << irqNum);
        outputb(MASTER_PIC_DATA_PORT, mask);
    } else {
        uint8_t mask = inputb(SLAVE_PIC_DATA_PORT);
        mask |= (1 << (irqNum - 8));
        outputb(SLAVE_PIC_DATA_PORT, mask);
    }
}

// unmasks the pic (enables the irq)
void unmask_pic(int irqNum) {
    if (irqNum < 8) {
        uint8_t mask = inputb(MASTER_PIC_DATA_PORT);
        mask &= ~(1 << irqNum);
        outputb(MASTER_PIC_DATA_PORT, mask);
    } else {
        uint8_t mask = inputb(SLAVE_PIC_DATA_PORT);
        mask &= ~(1 << (irqNum - 8));
        outputb(SLAVE_PIC_DATA_PORT, mask);
    }
}



uint16_t RetrieveIRQRegisters() {
    outputb(MASTER_PIC_COMMAND_PORT, PIC_READ_IRR);
    outputb(SLAVE_PIC_COMMAND_PORT, PIC_READ_IRR);
    return (inputb(SLAVE_PIC_DATA_PORT) << 8) | inputb(MASTER_PIC_DATA_PORT);
}

uint16_t RetrieveISRRegisters() {
    outputb(MASTER_PIC_COMMAND_PORT, PIC_READ_ISR);
    outputb(SLAVE_PIC_COMMAND_PORT, PIC_READ_ISR);
    return (inputb(SLAVE_PIC_DATA_PORT) << 8) | inputb(MASTER_PIC_DATA_PORT);
}


void DisablePic(void) {
    outputb(MASTER_PIC_DATA_PORT, 0xff);
    outputb(SLAVE_PIC_DATA_PORT, 0xff);
}