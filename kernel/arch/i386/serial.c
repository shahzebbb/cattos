#include <kernel/serial.h>
#include <string.h>

#include "io_ports.h"

#define PORT 0x3f8 // COM1

static int is_transmit_empty() {
    return inb(PORT + 5) & 0x20;
}

int serial_initialise() {
    outb(PORT + 1, 0x00); // Disable all interrupts
    outb(PORT + 3, 0x80); // Enable DLAB
    
    outb(PORT + 0, 0x03); // Set least significant byte for baud rate
    outb(PORT + 1, 0x00); // Set most significant byte for baud rate
                          // Baud rate is now set to 115200 / 3 = 38400 baud
                          // baud == ticks per second

    outb(PORT + 3, 0x03); // Bit 0-1: Characters (8)
                          // Bit 2: Stop Bits (Turned on)
                          // Bits 5-3: Parity Bits (Turned Off)
                          // Bit 7: DLAB (disabled)
    
    outb(PORT + 2, 0xC7); // Enable FIFO, Clear the queues, trigger interrupt
                          // 14 bits (this is relevant because we have 
                          // interrupts turned off but osdev did it so I follow
    
    outb(PORT + 4, 0x1E); // Enable loopback mode for testing and set other
                          // bits (which is not necessary so not sure why we
                          // do it?)
    
    outb(PORT + 0, 0xAE); // Test serial chip (send byte 0xAE and check if 
                          // serial returns the same byte)
    
    // Check if serial is faulty (i.e. not same byte received as sent)
    if (inb(PORT + 0) != 0xAE) {
        return 1;
    }

    // If serial is not faulty set it in normal operation mode
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    outb(PORT + 4, 0x0F);        
    return 0;
}

void serial_putchar(char c) {
    while (is_transmit_empty() == 0);

    outb(PORT, c);
}

void serial_write(const char* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        serial_putchar(data[i]);
    }
}

void serial_writestring(const char* data) {
    serial_write(data, strlen(data));
}
