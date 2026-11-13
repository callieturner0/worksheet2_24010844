#include "interrupts.h"
#include "pic.h"
#include "io.h"
#include "framebuffer.h"
#include "keyboard.h"

#define INTERRUPTS_DESCRIPTOR_COUNT 256
#define INTERRUPTS_KEYBOARD 33
#define INPUT_BUFFER_SIZE 256

// Circular input buffer
u8int input_buffer[INPUT_BUFFER_SIZE];

volatile u32int buffer_read_index = 0;
volatile u32int buffer_write_index = 0;

// IDT structures
struct IDTDescriptor idt_descriptors[INTERRUPTS_DESCRIPTOR_COUNT];
struct IDT idt;

// Initialise an IDT descriptor
void interrupts_init_descriptor(s32int index, u32int address)
{
    idt_descriptors[index].offset_high = (address >> 16) & 0xFFFF;
    idt_descriptors[index].offset_low = address & 0xFFFF;

    idt_descriptors[index].segment_selector = 0x08;
    idt_descriptors[index].reserved = 0x00;

    idt_descriptors[index].type_and_attr =
        (0x01 << 7) |
        (0x00 << 6) |
        (0x00 << 5) |
        0x0E;
}

// Add a character to the input buffer
void buffer_add(u8int character)
{
    u32int next_index =
        (buffer_write_index + 1) % INPUT_BUFFER_SIZE;

    if (next_index != buffer_read_index)
    {
        input_buffer[buffer_write_index] = character;
        buffer_write_index = next_index;
    }
}

// Get a character from the input buffer
int getc(void)
{
    u8int character;
    u32int flags;

    // Save the current interrupt state and disable interrupts
    asm volatile(
        "pushfl\n\t"
        "popl %0\n\t"
        "cli"
        : "=r"(flags)
        :
        : "memory");

    // Check whether the buffer is empty
    if (buffer_read_index == buffer_write_index)
    {
        // Restore the previous interrupt state
        asm volatile(
            "pushl %0\n\t"
            "popfl"
            :
            : "r"(flags)
            : "memory", "cc");

        return -1;
    }

    // Retrieve the next character
    character = input_buffer[buffer_read_index];

    // Move the read index forward
    buffer_read_index =
        (buffer_read_index + 1) % INPUT_BUFFER_SIZE;

    // Restore the previous interrupt state
    asm volatile(
        "pushl %0\n\t"
        "popfl"
        :
        : "r"(flags)
        : "memory", "cc");

    return character;
}

// Read a line of input from the buffer
int readline(char *buffer, u32int size)
{
    u32int index = 0;
    int character;

    if (size == 0)
    {
        return -1;
    }

    // Keep reading until Enter is pressed
    while (1)
    {
        character = getc();

        // Wait for another keyboard character
        if (character == -1)
        {
            continue;
        }

        // Stop when Enter is pressed
        if (character == '\n')
        {
            break;
        }

        // Handle Backspace
        if (character == '\b')
        {
            if (index > 0)
            {
                index--;
            }

            continue;
        }

        // Store the character if there is space
        if (index < size - 1)
        {
            buffer[index] = (char)character;
            index++;
        }
    }

    buffer[index] = '\0';

    return index;
}

// Install the Interrupt Descriptor Table
void interrupts_install_idt(void)
{
    interrupts_init_descriptor(
        INTERRUPTS_KEYBOARD,
        (u32int)interrupt_handler_33);

    idt.address = (u32int)&idt_descriptors;
    idt.size = sizeof(struct IDTDescriptor) *
                   INTERRUPTS_DESCRIPTOR_COUNT -
               1;

    load_idt((u32int)&idt);

    // Remap the PIC
    pic_remap(PIC_1_OFFSET, PIC_2_OFFSET);

    // Mask all interrupts except keyboard IRQ1
    outb(0x21, 0xFD);
    outb(0xA1, 0xFF);
}

// Handle hardware interrupts
void interrupt_handler(
    __attribute__((unused)) struct cpu_state *cpu,
    u32int interrupt,
    __attribute__((unused)) struct stack_state *stack)
{
    u8int input;
    u8int ascii;

    switch (interrupt)
    {
    case INTERRUPTS_KEYBOARD:

        while (inb(0x64) & 1)
        {
            input = keyboard_read_scan_code();

            // Only process key press codes
            if (!(input & 0x80))
            {
                if (input <= KEYBOARD_MAX_ASCII)
                {
                    ascii = keyboard_scan_code_to_ascii(input);

                    if (ascii != 0)
                    {
                        // Backspace
                        if (ascii == '\b')
                        {
                            buffer_add('\b');
                            fb_backspace();
                        }

                        // Enter
                        else if (ascii == '\n')
                        {
                            buffer_add('\n');
                            fb_newline();
                        }

                        // Regular character
                        else
                        {
                            buffer_add(ascii);
                            fb_write_char(ascii);
                        }
                    }
                }
            }
        }

        pic_acknowledge(interrupt);
        break;

    default:
        break;
    }
}