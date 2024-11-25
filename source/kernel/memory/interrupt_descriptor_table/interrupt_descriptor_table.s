    .global _idt_load
    .extern _pIDT      // Declare the external symbol _pIDT (which will point to the IDT descriptor)

_idt_load:
    lidt [ _pIDT ]    // Load the IDT using the address of _pIDT
    ret               // Return from the function
