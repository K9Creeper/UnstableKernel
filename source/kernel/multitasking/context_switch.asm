;;; ------------------
;;; context_switch.asm
;;; @brief This file defines the functions for multitasking, context switching.

global switch_stack
switch_stack:
    mov ebp, [esp + 4]

    mov ecx, [ebp + 28]
    mov edx, [ebp + 24]
    mov ebx, [ebp + 20]
    mov esi, [ebp + 8]
    mov edi, [ebp + 4]

    ; jump to usermode
    mov ax, 0x23
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; mov esp to eax
    mov eax, [ebp + 16]
    ; push data sec
    push 0x23
    ; push esp
    push eax
    ; move eflags
    mov eax, [ebp + 52]
    ; push flags
    push eax
    
    ; push code sec
    push 0x1b

    ; push the function to jump or place to spawn from
    mov eax, [ebp + 44]
    push eax

    ; respawn eax
    mov eax, [ebp + 32]

    ; respawn ebp
    mov ebp, [ebp + 12]

    iret 