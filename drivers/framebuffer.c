#include "framebuffer.h"

#define FB_WIDTH 80
#define FB_HEIGHT 25

// Framebuffer memory address
volatile unsigned short *fb = (unsigned short *)0x000B8000;

// Cursor position
static unsigned short cursor_x = 0;
static unsigned short cursor_y = 0;

// Text colour
static unsigned char colour = 0x0F;

// Move the cursor
void fb_move(unsigned short x, unsigned short y)
{
    if (x < FB_WIDTH && y < FB_HEIGHT)
    {
        cursor_x = x;
        cursor_y = y;
    }
}

// Change text colour
void fb_set_color(unsigned char foreground, unsigned char background)
{
    colour = (background << 4) | (foreground & 0x0F);
}

// Print one character
void fb_putchar(char c)
{
    if (c == '\n')
    {
        cursor_x = 0;
        cursor_y++;
    }
    else
    {
        unsigned short position = cursor_y * FB_WIDTH + cursor_x;
        fb[position] = ((unsigned short)colour << 8) | c;
        cursor_x++;
    }

    if (cursor_x >= FB_WIDTH)
    {
        cursor_x = 0;
        cursor_y++;
    }

    if (cursor_y >= FB_HEIGHT)
    {
        cursor_y = 0;
    }
}

// Print a character from keyboard input
void fb_write_char(char c)
{
    fb_putchar(c);
}

// Move the cursor to a new line
void fb_newline(void)
{
    cursor_x = 0;
    cursor_y++;

    if (cursor_y >= FB_HEIGHT)
    {
        cursor_y = 0;
    }
}

// Remove the previous character
void fb_backspace(void)
{
    if (cursor_x > 0)
    {
        cursor_x--;
    }
    else if (cursor_y > 0)
    {
        cursor_y--;
        cursor_x = FB_WIDTH - 1;
    }
    else
    {
        return;
    }

    unsigned short position = cursor_y * FB_WIDTH + cursor_x;
    fb[position] = ((unsigned short)colour << 8) | ' ';
}

// Print a string
void fb_write(char *text)
{
    while (*text)
    {
        fb_putchar(*text);
        text++;
    }
}

// Print a number
void fb_write_dec(int number)
{
    char buffer[12];
    int i = 0;

    if (number == 0)
    {
        fb_putchar('0');
        return;
    }

    if (number < 0)
    {
        fb_putchar('-');
        number = -number;
    }

    while (number > 0)
    {
        buffer[i++] = '0' + (number % 10);
        number = number / 10;
    }

    while (i > 0)
    {
        fb_putchar(buffer[--i]);
    }
}

// Clear the screen
void fb_clear(void)
{
    unsigned short blank = ((unsigned short)colour << 8) | ' ';

    for (int i = 0; i < FB_WIDTH * FB_HEIGHT; i++)
    {
        fb[i] = blank;
    }

    cursor_x = 0;
    cursor_y = 0;
}

// Initialise the framebuffer
void fb_init(void)
{
    fb_set_color(15, 0);
    fb_clear();
}