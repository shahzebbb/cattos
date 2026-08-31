#include <string.h>

void* memset(void* ptr, int value, size_t size) {
    unsigned char* p = (unsigned char*) ptr;
    unsigned char value_byte = (unsigned char) value;
    for (size_t i = 0; i < size; i++)
        p[i] = value_byte;
    return ptr;
}
