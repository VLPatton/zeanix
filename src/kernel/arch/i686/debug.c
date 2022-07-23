#ifdef __K_DEBUG
#include <serial.h>

int getDebugChar() {
    return (int)(unsigned char)serial_read();
}

void putDebugChar(int ic) {
    serial_putc((char)ic);
}

#endif
