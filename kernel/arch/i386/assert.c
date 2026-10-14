#include <stdio.h>
#include <kernel/panic.h>

void assertion_failed(
    const char* expression,
    const char * file,
    const char *function
) {
    printf("ASSERTION FAILED\n");
    printf("Expression: %s\n", expression);
    printf("File: %s\n", file);
    printf("Function: %s\n", function);

    panic("Assertion failed");
}
