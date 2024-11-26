    .global loadPageDirectory
    .global enablePaging
    .global disablePaging

loadPageDirectory:
    push %ebp
    mov %esp, %ebp
    mov 8(%esp), %eax
    mov %eax, %cr3
    mov %ebp, %esp
    pop %ebp
    ret

enablePaging:
    push %ebp
    mov %esp, %ebp
    mov %cr0, %eax
    or $0x80000000, %eax
    mov %eax, %cr0
    mov %ebp, %esp
    pop %ebp
    ret

disablePaging:
    push %ebp           
    mov %esp, %ebp      
    mov %cr0, %eax      
    and $~(0x80000000), %eax 
    mov %eax, %cr0       
    mov %ebp, %esp      
    pop %ebp            
    ret                 