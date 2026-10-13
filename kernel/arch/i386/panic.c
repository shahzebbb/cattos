#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/serial.h>
#include <kernel/panic.h>

void panic(const char* message) {
    if (is_serial_ready) {
        serial_writestring("KERNEL PANIC: ");
        serial_writestring(message);
        serial_writestring("\n");
    }

    if (is_terminal_ready)
        printf("KERNEL PANIC: %s\n", message);

    __asm__ volatile("cli");
    for (;;) {
        __asm__ volatile("hlt");
    }
}
