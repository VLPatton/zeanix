#ifndef _K_SERIAL_H
#define _K_SERIAL_H 1

#include <stdint.h>
#include <stddef.h>

int serial_init();      // Init Serial controller (used mostly for debug)
int serial_received();  // Determine if there are more bytes to be received on COM1
char serial_read();     // Read a byte of data from COM1
int serial_empty();     // Determine if more bytes need to be read
void serial_putc(char); // Send a byte of data to COM1

#endif
