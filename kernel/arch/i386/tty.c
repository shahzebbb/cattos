#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>

#include "vga.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;

// Define global variables to keep track of current location of terminal
typedef struct {
    size_t row;
    size_t column;
    uint8_t color;
    uint16_t* buffer;
} Terminal;

static Terminal terminal;

/*  ------------------- Helper functions -------------------*/ 
/* Print char `c` with the specified color and location to termianl. */
static void terminal_putentryat(
    unsigned char uc, 
    size_t x, 
    size_t y, 
    uint8_t color, 
    uint16_t* buffer
) {
    const size_t index = y * VGA_WIDTH + x;
    buffer[index] = vga_entry(uc, color);
}

/* Scroll terminal by copying all chars up one. */
static void terminal_scroll(Terminal* terminal) {
    for (size_t y = 1; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            const size_t index_prev = (y-1) * VGA_WIDTH + x;
            terminal->buffer[index_prev] = terminal->buffer[index]; 
        }
    }

    for (size_t x = 0; x < VGA_WIDTH; x++) {
        const size_t index = (VGA_HEIGHT-1) * VGA_WIDTH + x;
        terminal->buffer[index] = vga_entry(' ', terminal->color); 
    }

    terminal->row = VGA_HEIGHT-1;
}

/* Move the terminal cursor to a new line. */
static void terminal_newline(Terminal* terminal) {
    terminal->column = 0;
    if (++terminal->row == VGA_HEIGHT)
        terminal_scroll(terminal);
}

/* Increment terminal cursor by one. */
static void terminal_incrementcursor(Terminal* terminal) {
    if (++terminal->column == VGA_WIDTH) {
        terminal->column = 0;
        if (++terminal->row == VGA_HEIGHT)
            terminal_scroll(terminal);
    }
}

/*  --------------------------------------------------------*/ 

/* Initialise terminal by replacing all characters on screen with a space
   character. */
void terminal_initialise(void) {
    terminal.row = 0;
    terminal.column = 0;
    terminal.color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal.buffer = VGA_MEMORY;
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal.buffer[index] = vga_entry(' ', terminal.color); 
        }
    }
}

/* Print the character on the terminal. */
void terminal_putchar(char c) {
    unsigned char uc = c;
    
    if (uc == '\n') {
        terminal_newline(&terminal);
        return;
    }

    terminal_putentryat(
        uc,
        terminal.column, 
        terminal.row,
        terminal.color,
        terminal.buffer
    );
    terminal_incrementcursor(&terminal);
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
