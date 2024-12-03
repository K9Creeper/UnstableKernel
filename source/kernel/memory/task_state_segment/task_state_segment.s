.global _flush_tss
.global _get_eip
  _flush_tss:
     mov $0x28, %ax
     ltr %ax
     ret
  _get_eip:
    pop %eax
    jmp %eax