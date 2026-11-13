; Generic Interrupt Handler

extern interrupt_handler

%macro no_error_code_interrupt_handler 1
global interrupt_handler_%1

interrupt_handler_%1:
    push dword 0
    push dword %1
    jmp common_interrupt_handler
%endmacro

%macro error_code_interrupt_handler 1
global interrupt_handler_%1

interrupt_handler_%1:
    push dword %1
    jmp common_interrupt_handler
%endmacro

common_interrupt_handler:

    ; Save CPU registers
    push eax
    push ebx
    push ecx
    push edx
    push ebp
    push esi
    push edi

    ; Pass stack_state pointer
    lea eax, [esp + 32]
    push eax

    ; Pass interrupt number
    push dword [esp + 32]

    ; Pass cpu_state pointer
    lea eax, [esp + 8]
    push eax

    ; Call C interrupt handler
    call interrupt_handler

    ; Remove arguments
    add esp, 12

    ; Restore CPU registers
    pop edi
    pop esi
    pop ebp
    pop edx
    pop ecx
    pop ebx
    pop eax

    ; Remove interrupt number and error code
    add esp, 8

    ; Return from interrupt
    iret

no_error_code_interrupt_handler 33