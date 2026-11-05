#include "../drivers/framebuffer.h"

// Functions from functions.c
extern int sum_of_three(int a, int b, int c);
extern int multiply_two(int a, int b);
extern int find_largest(int a, int b, int c);

// Main kernel function
void kernel_main(void)
{
    // Initialise the screen
    fb_init();

    // Display a heading
    fb_set_color(15, 0);
    fb_write("My Tiny Operating System");
    fb_putchar('\n');

    // Display a message
    fb_set_color(10, 0);
    fb_write("Framebuffer driver is working!");
    fb_putchar('\n');

    // Display a number
    fb_set_color(14, 0);
    fb_write("Test number: ");
    fb_write_dec(12345);
    fb_putchar('\n');

    // Move the cursor
    fb_move(10, 10);
    fb_write("Hello from position 10, 10!");

    // Test the C functions
    int result1 = sum_of_three(10, 20, 30);
    int result2 = multiply_two(5, 6);
    int result3 = find_largest(15, 40, 25);

    // Display the results
    fb_move(0, 12);

    fb_set_color(15, 0);
    fb_write("Sum result: ");
    fb_write_dec(result1);
    fb_putchar('\n');

    fb_write("Multiply result: ");
    fb_write_dec(result2);
    fb_putchar('\n');

    fb_write("Largest result: ");
    fb_write_dec(result3);
    fb_putchar('\n');

    // Test clearing the screen
    fb_clear();
    fb_write("Screen cleared successfully!");

    // Keep the kernel running
    while (1)
    {
    }
}