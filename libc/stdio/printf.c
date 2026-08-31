#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

/* Returns false if EOF is reached in data otherwise true */
static bool print(const char* data, size_t length) {
    const unsigned char* bytes = (const unsigned char*) data;
    for (size_t i = 0; i < length; i++)
        if (putchar(bytes[i]) == EOF)
            return false;
    return true;
}

int printf(const char* restrict format, ...) {
    /* va =  variable arguments */
    va_list parameters;
    va_start(parameters, format);

    int written = 0;

    while (*format != '\0') {
        size_t maxrem = INT_MAX - written;

        /* Print none formatted chars.
        %% are printed as %. For example 100%% is 100%. */
        if (format[0] != '%' || format[1] == '%') {
            if (format[0] == '%')
                format++;

            size_t amount = 1;
            
            while (format[amount] && format[amount] != '%')
                amount++;

            if (amount > maxrem) {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }

            if (!print(format, amount))
                return -1;

            format += amount;
            written += amount;
            continue;
        }

        /* If we reach this point then we are at some char
           starting with %. */
        const char* format_begun_at = format++;

        if (*format == 'c') {
            format++;
            
            /* va_arg promotes char to int automatically. */
            char c = (char) va_arg(parameters, int);

            if (!maxrem) {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }

            if (!print(&c, sizeof(c)))
                return -1;
            
            written++;
        } else if (*format == 's') {
            format++;
            const char* str = va_arg(parameters, const char*);
            size_t len = strlen(str);
            
            if (!maxrem) {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }

            if (!print(str, len))
                return -1;

            written += len;
        } else {
            format = format_begun_at;
            size_t len = strlen(format);

            if (!maxrem) {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }

            if (!print(format, len))
                return -1;

            written += len;
            format += len;
        }

    va_end(parameters);
    return written;

    }
}
