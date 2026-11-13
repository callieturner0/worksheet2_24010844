#include "pic.h"
#include "io.h"

// Remap the Programmable Interrupt Controller
void pic_remap(s32int offset1, s32int offset2)
{
    unsigned char a1, a2;

    // Save the current masks
    a1 = inb(PIC_1_DATA);
    a2 = inb(PIC_2_DATA);

    // Start PIC initialisation
    outb(PIC_1_COMMAND, PIC_ICW1_INIT | PIC_ICW1_ICW4);
    outb(PIC_2_COMMAND, PIC_ICW1_INIT | PIC_ICW1_ICW4);

    // Set interrupt offsets
    outb(PIC_1_DATA, offset1);
    outb(PIC_2_DATA, offset2);

    // Configure master and slave PICs
    outb(PIC_1_DATA, 4);
    outb(PIC_2_DATA, 2);

    // Set 8086 mode
    outb(PIC_1_DATA, PIC_ICW4_8086);
    outb(PIC_2_DATA, PIC_ICW4_8086);

    // Restore the original masks
    outb(PIC_1_DATA, a1);
    outb(PIC_2_DATA, a2);
}

// Acknowledge an interrupt
void pic_acknowledge(u32int interrupt)
{
    if (interrupt >= PIC_2_OFFSET && interrupt <= PIC_2_END)
    {
        outb(PIC_2_COMMAND_PORT, PIC_ACKNOWLEDGE);
    }

    outb(PIC_1_COMMAND_PORT, PIC_ACKNOWLEDGE);
}