#ifndef _KERNEL_SERIAL_H
#define _KERNEL_SERIAL_H

#include <stddef.h>
#include <stdbool.h>

int serial_initialise(void);
void serial_putchar(char c);
void serial_write(const char* data, size_t size);
void serial_writestring(const char* data);
bool is_serial_ready();

#endif
