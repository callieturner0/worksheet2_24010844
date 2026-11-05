# Operating Systems – Worksheet 2 Part 1

## Introduction

This project involves developing a small operating system using Assembly and C. The aim was to understand how a computer starts an operating system, how Assembly can call C functions, and how a framebuffer can be used to display text.

The project was developed using the UWE CSCT Cloud environment, with NASM, GCC, Make and QEMU.

---

## Task 1 – Starting a Tiny Operating System

For Task 1, I created the basic structure needed for the operating system. This included the `source`, `drivers` and `iso` directories, along with a Makefile and linker script.

I created an Assembly boot loader which allows the operating system to start. The initial kernel was designed to place the value `0xCAFEBABE` into the EAX register.

The boot loader also sets up a stack before calling the kernel.

Example:

```asm
mov esp, stack_top
cld
call kernel_main
```

The stack is required so that the C kernel can run correctly.

I also created a linker script to organise the kernel in memory and used GRUB to boot the operating system through a bootable ISO file.

**Screenshot – Project structure and files**

[Insert screenshot here]

**Screenshot – QEMU successfully booting the operating system**

[Insert screenshot here]

---

## Task 2 – Calling C from Assembly

For Task 2, I extended the operating system so that it could run C code instead of only Assembly.

I updated the Makefile to compile the C source files using GCC and link them with the Assembly loader.

I created `functions.c`, which contains three functions:

* `sum_of_three()` adds three numbers together.
* `multiply_two()` multiplies two numbers.
* `find_largest()` finds the largest of three numbers.

For example, the largest number function compares the values and updates the largest variable when it finds a bigger number.

```c
int find_largest(int a, int b, int c)
{
    int largest = a;

    if (b > largest)
        largest = b;

    if (c > largest)
        largest = c;

    return largest;
}
```

I tested the functions using the kernel and also checked that the functions had been compiled and linked into the kernel.

The test results were:

| Function | Test         | Result |
| -------- | ------------ | -----: |
| Sum      | 10 + 20 + 30 |     60 |
| Multiply | 5 × 6        |     30 |
| Largest  | 15, 40, 25   |     40 |

**Screenshot – C functions**

[Insert screenshot here]

**Screenshot – Function test results**

[Insert screenshot here]

---

## Task 3 – Framebuffer Driver

For Task 3, I developed a framebuffer driver to allow the operating system to display text.

A framebuffer uses memory-mapped I/O. This means that writing data to a particular memory address updates the display.

The framebuffer memory address used in this project is:

```c
0x000B8000
```

The screen uses an 80 × 25 text layout. Each screen position stores a character and its colour information.

### Framebuffer API

I created `framebuffer.c` and `framebuffer.h` inside the `drivers` directory.

The header file contains the function declarations, allowing the kernel to use the framebuffer functions.

The driver includes the following functions:

| Function               | Purpose                                 |
| ---------------------- | --------------------------------------- |
| `fb_init()`            | Initialises the framebuffer             |
| `fb_move(x, y)`        | Moves the cursor to a screen position   |
| `fb_putchar(c)`        | Displays one character                  |
| `fb_write(text)`       | Displays a string                       |
| `fb_write_dec(number)` | Displays a decimal number               |
| `fb_set_color(fg, bg)` | Changes the text and background colours |
| `fb_clear()`           | Clears the screen                       |

### Writing to framebuffer memory

The framebuffer is accessed using a pointer to the memory address:

```c
volatile unsigned short *fb =
    (unsigned short *)0x000B8000;
```

The `volatile` keyword is used because the memory is connected to hardware and its contents can change outside normal program operations.

To display a character, the driver combines the character with its colour attribute:

```c
fb[position] = ((unsigned short)colour << 8) | c;
```

This allows the driver to control both the character displayed and its colour.

### Cursor positioning

I added a function to move the cursor using an x and y position.

```c
void fb_move(unsigned short x, unsigned short y)
{
    if (x < FB_WIDTH && y < FB_HEIGHT)
    {
        cursor_x = x;
        cursor_y = y;
    }
}
```

This checks that the requested position is inside the screen boundaries before updating the cursor coordinates.

### Testing the driver

I updated `kernel.c` to test the framebuffer functions.

The kernel displays strings, numbers and different text colours. I also tested moving the cursor to position `(10, 10)`.

I added a screen-clearing test using:

```c
fb_clear();
fb_write("Screen cleared successfully!");
```

This clears the previous contents and displays a new message from the beginning of the screen.

I updated the Makefile to compile `drivers/framebuffer.c` and link the resulting object file into the kernel.

### Running with QEMU

I changed the QEMU run command to use Curses mode, as required by the worksheet.

I also enabled a Telnet monitor on port `45454`. This allows QEMU to be controlled from another terminal.

To connect to the monitor:

```bash
telnet localhost 45454
```

The `quit` command can then be used to close QEMU.

**Screenshot – Framebuffer header file**

[Insert screenshot here]

**Screenshot – Framebuffer driver implementation**

[Insert screenshot here]

**Screenshot – Kernel testing the framebuffer functions**

[Insert screenshot here]

**Screenshot – QEMU showing the test results**

[Insert screenshot here]

**Screenshot – Telnet framebuffer memory check**

[Insert screenshot here]

---

## Task 4 – Documentation

This README documents the development of the operating system across Tasks 1, 2 and 3.

It includes explanations of the main implementation decisions, selected code examples and screenshots showing the operating system and its functions working.

The complete source code is included in the repository.
