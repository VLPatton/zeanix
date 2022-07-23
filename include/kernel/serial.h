#ifndef _K_SERIAL_H
#define _K_SERIAL_H 1

#include <stdint.h>
#include <stddef.h>

int serial_init();
int serial_received();
char serial_read();
int serial_empty();
void serial_putc(char);

#endif
