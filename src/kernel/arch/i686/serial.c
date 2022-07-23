#include <serial.h>
#include <arch/i686/CPUID.h>

const uint32_t port = 0x3F8;

int serial_init() {
    outb(port + 1, 0x00);   // Disable interrupts
    outb(port + 3, 0x80);   // Enable DLAB
    outb(port + 0, 0x03);   // Set baud rate to 38400
    outb(port + 1, 0x00);   // ^
    outb(port + 3, 0x03);   // 8 bits, no parity, one stop bit
    outb(port + 2, 0xC7);   // Enable FIFO, clear them, with 14-byte threshold
    outb(port + 4, 0x0B);   // IRQs enabled, RTS/DSR set
    outb(port + 4, 0x1E);   // Set in loopback mode, test the serial chip
    outb(port + 0, 0xAE);   // Test serial chip (send byte 0xAE and check if serial returns same byte)

    if (inb(port + 0) != 0xAE)
        return -1;
    outb(port + 4, 0x0F);
    return 0;
}

int serial_received() {
   return inb(port + 5) & 1;
}
 
char serial_read() {
   while (serial_received() == 0);
 
   return inb(port);
}

int serial_empty() {
   return inb(port + 5) & 0x20;
}
 
void serial_putc(char a) {
   while (serial_empty() == 0);
 
   outb(port, a);
}
