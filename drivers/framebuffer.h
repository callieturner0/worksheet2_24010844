#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

// Initialise the framebuffer
void fb_init(void);

// Move the cursor
void fb_move(unsigned short x, unsigned short y);

// Print a character
void fb_putchar(char c);

// Print a string
void fb_write(char *text);

// Print a number
void fb_write_dec(int number);

// Change text colour
void fb_set_color(unsigned char foreground, unsigned char background);

// Clear the screen
void fb_clear(void);

// Keyboard input functions
void fb_write_char(char c);
void fb_newline(void);
void fb_backspace(void);

#endif