#include <stdint.h>
#include <stdio.h>

/* Format excepted by x86 when loading the IDT using lidt */
typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute((packed)) idtr_t;

/* 8-byte interrupt gate descriptor */
typedef struct {
    uint16_t isr_low;    // Lower 16 bits of the ISR's address
    uint16_t kernel_cs;  // GDT segment selector that the CPU will
                         // load into CS before calling the ISR
    uint8_t  reserved;   // Must be zero
    uint8_t  attributes; // Gate type, privilege and present bit
    uint16_t isr_high;   // Upper 16 bits of the ISR's address
} __attribute((packed)) idt_entry_t;


/* Define the IDT in memory as a variable, osdev says to align to 16 bytes. */
__attribute__((aligned(0x10)))
static idt_entry_t idt[256];

static idtr_t idtr;

extern void isr_stub_3(void);

/* Creates an entry in the IDT table at location `vector`.
   The location of the function that runs is in `isr`. */
static void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    idt_entry_t* descriptor = &idt[vector];

    descriptor->isr_low = (uint32_t) isr & 0xFFFF;
    descriptor->kernel_cs = 0x08;
    descriptor->attributes = flags;
    descriptor->isr_high = (uint32_t)isr >> 16;
    descriptor->reserved = 0;
}


/* Initialise the IDT. For now we only initialise interrupt 3. */
void idt_init() {
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * 256 - 1;
    
    idt_set_descriptor(3, (void *)isr_stub_3, 0x8E);
    
    __asm__ volatile(
        "lidt %0" 
        : : 
        "m"(idtr)
    );
}

/* This function is called by interrupt 3. */
void exception_handler() {
    printf("Breakpoint caught!\n");
}
