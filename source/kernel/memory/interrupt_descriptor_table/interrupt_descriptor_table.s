.global _idt_load
.extern _pIDT
  
_idt_load:
    lidt [ _pIDT ]
    ret