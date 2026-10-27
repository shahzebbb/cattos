#include <stdint.h>

#include <kernel/panic.h>

/* Macros for access segment bits we will put in the segment byte */
#define ACCESS_SEG_DESCRIPTOR_TYPE(x) ((x) << 0x04) // Descriptor type (0 for 
                                                    // system, 1 for code/data)
#define ACCESS_SEG_PRESENT(x)         ((x) << 0x07) // Present bit
#define ACCESS_SEG_PRIVILEGE(x)       (((x) & 0x03) << 0x05) // Set privilege 
                                                             // level (0 - 3)
#define ACCESS_SEG_DATA_RDWR 0x02 // Read and Write bits
#define ACCESS_SEG_CODE_EXRD 0x0A // Read and Execute bits

/* -------------------------------------------------------------- */

/* Macros for flag segment bits we will put in the segment byte */
#define FLAG_SEG_SAVL(x)        ((x) << 0x00) // Available for system use
#define FLAG_SEG_LONG(x)        ((x) << 0x01) // Long mode
#define FLAG_SEG_SIZE(x)        ((x) << 0x02) // Size (0 for 16-bit, 1 for 32)
#define FLAG_SEG_GRANULARITY(x) ((x) << 0x03) // Granularity (0B for 1B - 1MB, 
                                              // 1 for 4KB - 4GB)

/* ------------------------------------------------------------ */

#define GDT_CODE_PL0_ACCESS ACCESS_SEG_DESCRIPTOR_TYPE(1)  | ACCESS_SEG_PRESENT(1) | \
                            ACCESS_SEG_PRIVILEGE(0)        | ACCESS_SEG_CODE_EXRD
#define GDT_DATA_PL0_ACCESS ACCESS_SEG_DESCRIPTOR_TYPE(1)  | ACCESS_SEG_PRESENT(1) | \
                            ACCESS_SEG_PRIVILEGE(0)        | ACCESS_SEG_DATA_RDWR
#define GDT_FLAG            FLAG_SEG_SAVL(0) | FLAG_SEG_LONG(0) | \
                            FLAG_SEG_SIZE(1) | FLAG_SEG_GRANULARITY(1)

// Store the gdt_list as a global variable                            
static uint8_t gdt_list[3][8];

extern void set_gdt(uint16_t limit, uint32_t base);

/* Create an entry in the gdt table located at target.
   target specifies the index we will put the entry at. */
static void create_gdt_entry(
    uint8_t* target, 
    uint32_t base, 
    uint32_t limit, 
    uint8_t access,
    uint8_t flag
) {
    if (limit > 0xFFFFF)
        panic("GDT cannot encode limits larger than 0xFFFFF");
    
    // Encode the limit
    target[0] = limit & 0xFF;
    target[1] = (limit >> 8) & 0xFF;
    target[6] = (limit >> 16) & 0x0F;

    // Encode the base
    target[2] = base & 0xFF;
    target[3] = (base >> 8) & 0xFF;
    target[4] = (base >> 16) & 0xFF;
    target[7] = (base >> 24) & 0xFF;

    // Encode access
    target[5] = access & 0xFF;

    // Encode flag
    target[6] |= (flag << 4) & 0xF0;

}

void gdt_init() {
    create_gdt_entry(gdt_list[0], 0, 0, 0, 0);
    create_gdt_entry(gdt_list[1], 0, 0x000FFFFF, GDT_CODE_PL0_ACCESS, GDT_FLAG);
    create_gdt_entry(gdt_list[2], 0, 0x000FFFFF, GDT_DATA_PL0_ACCESS, GDT_FLAG);
    set_gdt(sizeof(gdt_list)-1, (uint32_t)gdt_list);
}
 