#ifndef _KERNEL_ASSERT_H
#define _KERNEL_ASSERT_H

void assertion_failed(
    const char* expression,
    const char* file,
    const char* function
);

#define assert(expr) \
    ((expr) ? (void)0 : assertion_failed(#expr, __FILE__, __func__))

#endif
