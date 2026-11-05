global loader
extern kernel_main

MAGIC_NUMBER equ 0x1BADB002
FLAGS        equ 0x0
CHECKSUM     equ -MAGIC_NUMBER

section .text
align 4

dd MAGIC_NUMBER
dd FLAGS
dd CHECKSUM

loader:
    cli

    ; Set up the stack
    mov esp, stack_top

    ; Prepare for C code
    cld

    ; Call the C kernel
    call kernel_main

.loop:
    jmp .loop

section .bss
align 16

stack_bottom:
    resb 16384
stack_top: