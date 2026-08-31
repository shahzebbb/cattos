#include <stdio.h>

#include <kernel/tty.h>

void kernel_main(void) {
    terminal_initialise();
    printf("Hello, kernel World!\n");
}
