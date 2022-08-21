#ifndef _K_TTY_H
#define _K_TTY_H 1

#include <stddef.h>
#include <stdint.h>
#include <string.h>

void tty_init(void);                    // Init the TTY
void tty_clear(void);                   // Clear TTY after init
void tty_putchar(char);                 // putchar() on TTY
int  tty_puts(const char*);             // puts() on TTY
void tty_setColor(uint8_t, uint8_t);    // Set Foreground and Background color of TTY text

// default VGA TTY colors
enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

#endif