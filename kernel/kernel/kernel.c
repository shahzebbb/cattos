#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/serial.h>
#include <kernel/panic.h>
#include <kernel/assert.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>

void kernel_main(void) {
    if (serial_initialise() == 1) {
        panic("Initialising serial output failed");
        return;
    }
        
    serial_writestring("Launching Kernel....\n");

    serial_writestring("Initialising GDT\n");
    gdt_init();

    serial_writestring("Initialising IDT\n");
    idt_init();

    serial_writestring("Initialising terminal\n");
    terminal_initialise();
    
    serial_writestring("Calling interrupt 3\n");
    __asm__ volatile("int3");
    
    printf("Hello, kernel World!\nHow are you?\n");

    serial_writestring("Exiting terminal...\n");
}
