#include <arch/i686/CPUID.h>

unsigned int cpuid_getVendorString() {
    unsigned int signature = 0;
    __get_cpuid_max(0, &signature); // Use GCC's built in __get_cpuid_max() to get the four byte signature of the vendor
    return signature;
}

void outb(uint16_t port, uint8_t data) {
    asm volatile (
        "outb %1, %0"
        : : "a"(data), "Nd" (port)
    );
}

void iowait(void) {
    outb(0x80, 0);
}

uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile (
        "inb %0, %1"
        : "=a" (ret)
        : "Nd" (port)
    );
    return ret;
}
