#include <string.h>

void* memmove(void* dstptr, void* srcptr, size_t size) {
    unsigned char* dst = (unsigned char*) dstptr;
    const unsigned char* src = (const unsigned char*) srcptr;

    if (dst < src) {
        for (size_t i = 0; i < size; i++)
            dst[i] = src[i];
    } else if (dst > src) {
        // use i != 0 since size_t is unsigned so if i is negative 
        // and we do -- it wraps to a huge value
        for (size_t i = size; i != 0; i--) 
            dst[i-1] = src[i-1];
    }

    return dstptr;
}
