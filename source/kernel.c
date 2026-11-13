#include "../drivers/framebuffer.h"
#include "../drivers/interrupts.h"
#include "../drivers/hardware_interrupt_enabler.h"

void kernel_main(void)
{
    char input[128];
    int length;

    // Initialise the framebuffer
    fb_init();

    fb_set_color(15, 0);

    fb_write("KERNEL STARTED");
    fb_putchar('\n');

    fb_write("Framebuffer is working!");
    fb_putchar('\n');

    // Install the interrupt descriptor table
    interrupts_install_idt();

    fb_write("IDT installed!");
    fb_putchar('\n');

    // Enable hardware interrupts
    enable_hardware_interrupts();

    fb_write("Hardware interrupts enabled!");
    fb_putchar('\n');

    // Terminal input loop
    while (1)
    {
        fb_write("myos> ");

        // Read a line of keyboard input
        length = readline(input, sizeof(input));

        if (length >= 0)
        {
            fb_write("You entered: ");
            fb_write(input);
            fb_putchar('\n');
        }
    }
}