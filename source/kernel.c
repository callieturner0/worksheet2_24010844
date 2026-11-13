#include "../drivers/framebuffer.h"
#include "../drivers/interrupts.h"
#include "../drivers/hardware_interrupt_enabler.h"

typedef void (*command_function)(char *args);

struct command
{
    char *name;
    command_function function;
};

// Check whether two strings are equal
int strings_equal(char *a, char *b)
{
    int i = 0;

    while (a[i] != '\0' && b[i] != '\0')
    {
        if (a[i] != b[i])
        {
            return 0;
        }

        i++;
    }

    return a[i] == '\0' && b[i] == '\0';
}

// Help command
void command_help(char *args)
{
    (void)args;

    fb_write("Available commands:");
    fb_putchar('\n');

    fb_write("help - Display available commands");
    fb_putchar('\n');

    fb_write("echo [text] - Display text");
    fb_putchar('\n');

    fb_write("clear - Clear the screen");
    fb_putchar('\n');

    fb_write("version - Display OS version");
    fb_putchar('\n');
}

// Echo command
void command_echo(char *args)
{
    if (args != 0)
    {
        fb_write(args);
    }

    fb_putchar('\n');
}

void command_clear(char *args)
{
    (void)args;
    fb_clear();
}

void command_version(char *args)
{
    (void)args;

    fb_write("MyOS version 1.0");
    fb_putchar('\n');
    fb_write("Cyber Security OS");
    fb_putchar('\n');
}

// Available commands
static struct command commands[] =
    {
        {"help", command_help},
        {"echo", command_echo},
        {"clear", command_clear},
        {"version", command_version}};

// Find and execute a command
void execute_command(char *input)
{
    char *command = input;
    char *args = 0;
    int i;

    // Ignore leading spaces
    while (*command == ' ')
    {
        command++;
    }

    // Ignore empty input
    if (*command == '\0')
    {
        return;
    }

    // Find the end of the command
    args = command;

    while (*args != '\0' && *args != ' ')
    {
        args++;
    }

    // Separate command from arguments
    if (*args == ' ')
    {
        *args = '\0';
        args++;

        while (*args == ' ')
        {
            args++;
        }
    }
    else
    {
        args = 0;
    }

    // Search for the command
    for (i = 0; i < sizeof(commands) / sizeof(commands[0]); i++)
    {
        if (strings_equal(command, commands[i].name))
        {
            commands[i].function(args);
            return;
        }
    }

    // Unknown command
    fb_write("Unknown command: ");
    fb_write(command);
    fb_putchar('\n');
}

void kernel_main(void)
{
    char input[128];
    int length;

    fb_init();
    fb_set_color(15, 0);

    fb_write("KERNEL STARTED");
    fb_putchar('\n');

    fb_write("Framebuffer is working!");
    fb_putchar('\n');

    interrupts_install_idt();

    fb_write("IDT installed!");
    fb_putchar('\n');

    enable_hardware_interrupts();

    fb_write("Hardware interrupts enabled!");
    fb_putchar('\n');

    while (1)
    {
        fb_write("myos> ");

        length = readline(input, sizeof(input));

        if (length >= 0)
        {
            execute_command(input);
        }
    }
}