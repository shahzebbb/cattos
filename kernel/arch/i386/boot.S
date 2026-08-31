# Declare constants for the multiboot header
.set ALIGN,     1<<0             # Tells bootloader to align loaded modules on 
                                 # page boundaries (we are not loading 
                                 # modules atm)

.set MEMINFO,   1<<1             # Ask bootloader to reutrn how memory is mapped
                                 # in RAM (i.e where the kernel is mapped, 
                                 # where is the bios is, etc.)

.set FLAGS,     ALIGN | MEMINFO  # This is the flag that actually goes into 
                                 # the bootloader, it tells the bootloader want 
                                 # we want (i.e. align and give memory info) 

.set MAGIC,     0x1BADB002       # Magic numbers that lets the bootloader find 
                                 # the mulitboot header

.set CHECKSHUM, -(MAGIC + FLAGS) # Checksum to prove that we are multiboot
                                 # (since random binary can also contain the
                                 # magic value) 

/*
We declare the Multiboot header. This is what the bootloader searches for.

The bootloader searches for the MAGIC value within the first 8 KiB of 
the kernel file.
It only checks addresses aligned on a 32-bit boundary, meaning every 4 bytes.

After finding MAGIC, the bootloader reads the next 32-bit value as FLAGS to see 
what the kernel is requesting. It then reads CHECKSUM and verifies that:

    MAGIC + FLAGS + CHECKSUM == 0

This header is in its own section so the linker script can force it to appear
within the first 8 Kib of the kernel file.
*/
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSHUM

/*
We need to define a stack because the Multiboot standard does not guarantee that
the bootloader sets up a valid stack for us. So the kernel must provide one.

Below we reserve 16384 bytes, which is 16 KiB, for a small kernel stack.

The stack is placed in .bss, which is for uninitialized memory. This means the
kernel file does not need to literally contain 16384 bytes for the stack 
(in the binary we just say that we need 16384 bytes when running this binary).

We align the stack to 16 bytes because that is what the C compiler expects
according to the ABI. If the stack is not aligned properly, compiler-generated
C code may behave incorrectly.

The stack grows downwards on x86, so later we set esp to stack_top, not
stack_bottom.
*/
.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top:

/*
We declare the .text section, where executable code goes.

We make _start global so the linker can see it and refer to it outside this 
file. The linker script uses _start as the entry point of the kernel, so this is 
where execution begins after the bootloader loads the kernel.

`.type _start, @function` marks _start as a function symbol in the ELF metadata.
This is mostly for the linker/debugging tools; it does not create code by itself.
*/
.section .text
.global _start
.type _start, @function
_start:
    # The bootloader has loaded the kernel into memory and jumped to _start.
    # At this point, we are running in 32-bit protected mode on x86.
    # As of now, interrupts and paging is disabled.

    # We setup a stack previously and now we activate that stack by setting the
    # stack pointer register, esp, to stack_top.
    mov $stack_top, %esp

    # Call global constructors
    call _init

    call kernel_main

    # Normally kernel_main should never return and should run forever.
    # So we halt the cpu if we ever do return.
    # We do the following
    # 1.) Disable interrupts
    # 2.) Halt the cpu forever. 
    #     Halt normally stops cpu execution unless woken up by an interrupt.
    #     In this case we disabled interrupts so it should halt interrupts forever.
    # 3.) Jump to the halt instruction if it ever wakes up due to a non-maskable
    #     interrupt occuring or due to system management mode.
    cli
1:  hlt
    jmp 1b

/*
Set the size of the _start symbol to the current location "." minus its start.
This is useful for debugging when we implement call tracing.
Call tracing is basically showing the chain of functions that led to some point.
*/
.size _start, . - _start


