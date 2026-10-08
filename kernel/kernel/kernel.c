#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/serial.h>

void kernel_main(void) {
    if (serial_initialise() == 1)
        return;
    
    serial_writestring("Launching Kernel....\n");

    serial_writestring("Initialising terminal\n");
    terminal_initialise();
    
    printf("Hello, kernel World!\nHow are you?\n");
    for (int i=0; i < 100; i++) {
        printf("hi\n");
    }

    serial_writestring("Exiting terminal...\n");
}
