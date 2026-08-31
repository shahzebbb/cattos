#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>

#include "vga.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;

// Define global variables to keep track of current location of terminal
static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;

// Helper functions
static void terminal_putentryat(unsigned char uc, uint8_t color, size_t x, size_t y);

/* Initialise terminal by replacing all characters on screen with a space
   character. */
void terminal_initialise(void) {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer = VGA_MEMORY;
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color); 
        }
    }
}

/* Print the character on the terminal. */
void terminal_putchar(char c) {
    unsigned char uc = c;
    terminal_putentryat(
        uc,
        terminal_color,
        terminal_column, 
        terminal_row
    );
    if (++terminal_column == VGA_WIDTH) {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT)
            terminal_row = 0;
    }
}

/* Print `size` number of characters from memory addr starting at `data` to 
   terminal. */
void terminal_write(const char *data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        terminal_putchar(data[i]);
    }
}

/* Print the string starting at memory addr `data` to terminal. */
void terminal_writestring(const char *data) {
    terminal_write(data, strlen(data));
}

/* Print char `c` with the specified color and location to termianl. */
static void terminal_putentryat(unsigned char uc, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(uc, color);
}
