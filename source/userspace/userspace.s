    .global jump_usermode
    .extern userspace_entry

jump_usermode:
    movw    $0x23, %ax          # ring 3 data selector with bottom 2 bits set for ring 3
    movw    %ax, %ds
    movw    %ax, %es
    movw    %ax, %fs
    movw    %ax, %gs            # SS is handled by iret

    # Set up the stack frame iret expects
    movl    %esp, %eax
    pushl   $0x23                # Data selector
    pushl   %eax                 # Current esp
    pushfl                       # EFLAGS
    pushl   $0x1B                # Code selector (ring 3 code with bottom 2 bits set)
    pushl   $userspace_entry     # Instruction address to return to
    iret
