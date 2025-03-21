;;; ------------------
;;; context_switch.asm
;;; @brief This file defines the functions for multitasking, context switching.

global context_switch
context_switch:
    ; Save base pointer
    mov ebp, [esp + 4]

    ; Restore general-purpose registers
    mov ecx, [ebp + 28]  ; ecx
    mov edx, [ebp + 24]  ; edx
    mov ebx, [ebp + 20]  ; ebx
    mov esi, [ebp + 8]   ; esi
    mov edi, [ebp + 4]   ; edi

    ; Set up segment registers for user mode (0x23 = User Data Segment)
    mov ax, 0x23
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Load ESP (User Stack Pointer)
    mov eax, [ebp + 16]  ; eax = user stack pointer
    push 0x23            ; User Data Segment
    push eax             ; Push ESP

    ; Load EFLAGS
    mov eax, [ebp + 52]  ; eax = eflags
    push eax             ; Push EFLAGS

    ; Load CS and EIP
    push 0x1B            ; User Code Segment (0x1B)
    mov eax, [ebp + 44]  ; eax = eip (entry point)
    push eax             ; Push EIP

    ; Restore EAX
    mov eax, [ebp + 32]

    ; Restore EBP
    mov ebp, [ebp + 12]

    ; Perform iret (Interrupt Return) to switch to user mode
    iret