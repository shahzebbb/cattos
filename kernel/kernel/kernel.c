#include <stdio.h>

#include <kernel/tty.h>

void kernel_main(void) {
    terminal_initialise();
    printf("Hello, kernel World!\nHow are you?");
    for (int i=0; i < 100; i++) {
        printf("hi\n");
    }
}
