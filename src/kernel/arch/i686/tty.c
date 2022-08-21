#include <tty.h>

const size_t _TTY_WIDTH = 80;                            // Maximum width of TTY
const size_t _TTY_HEIGHT = 25;                           // Maximum height of TTY
uint16_t* const _TTY_BUFFER_ADDR = (uint16_t*)0xB8000;   // Default buffer addr in i686

uint16_t* tty_textBuffer;    // Text buffer pointer with color attrib
size_t tty_currentRow;       // Current row to print on
size_t tty_currentCol;       // Current column to print on
uint8_t tty_color;           // Current set color (does not affect previously printed chars)

// Init TTY
void tty_init(void) {
    tty_currentCol = 0;
    tty_currentRow = 0;
    tty_textBuffer = _TTY_BUFFER_ADDR;
    tty_color = VGA_COLOR_LIGHT_GREY | VGA_COLOR_BLACK << 4;

    for (size_t y = 0; y < _TTY_HEIGHT; y++) {
        for (size_t x = 0; x < _TTY_WIDTH; x++) {
            const size_t index = y * _TTY_WIDTH + x;
            tty_textBuffer[index] = (uint16_t)tty_color<< 8 | (uint16_t)' ';
        }
    }
}

// putchar() on TTY
void tty_putchar(char c) {
    switch (c) {
        case 0x0A:  // '\n'
            tty_currentCol = 0;
            if (++tty_currentRow == _TTY_HEIGHT) {
                memmove(tty_textBuffer, tty_textBuffer + _TTY_WIDTH, (_TTY_HEIGHT - 1) * _TTY_WIDTH);
                tty_currentRow = _TTY_HEIGHT - 1;
            }
            break;
        default:
            const size_t index = tty_currentRow * _TTY_WIDTH + tty_currentCol;
            tty_textBuffer[index] = (uint16_t)tty_color<< 8 | (uint16_t)c;
            if (++tty_currentCol == _TTY_WIDTH) {
                tty_currentCol = 0;
                if (++tty_currentRow == _TTY_HEIGHT) {
                    memmove(tty_textBuffer, tty_textBuffer + _TTY_WIDTH, (_TTY_HEIGHT - 1) * _TTY_WIDTH);
                    tty_currentRow = _TTY_HEIGHT - 1;
                }
            }
            break;
    }
}

// puts() on TTY
int tty_puts(const char* data) {
    for (int i = 0; i < (int)strlen(data); i++)
        tty_putchar(data[i]);
    return (int)strlen(data);
}