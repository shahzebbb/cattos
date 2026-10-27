.section .data
gdtr:
    .word 0
    .long 0

.section .text
.global set_gdt

set_gdt:
    movw 4(%esp), %ax
    movw %ax, gdtr

    movl 8(%esp), %eax
    movl %eax, gdtr+2

    lgdt gdtr
    ljmp $0x08, $reload_cs
    
reload_cs:
    movw $0x10, %ax

    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    
    ret
