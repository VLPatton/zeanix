#ifndef _K_ARCH_I686_CPUID_H
#define _K_ARCH_I686_CPUID_H 1

#include <cpuid.h>
#include <stdint.h>

/* Vendor strings from manufacturers */
#define CPUID_VENDOR_AMD   0x68747541
#define CPUID_VENDOR_INTEL 0x756e6547

unsigned int cpuid_getVendorString();   // Retrieve vendor string (TODO: get actual string and not first four chars)

uint8_t inb(uint16_t);
void outb(uint16_t, uint8_t);
void iowait(void);

#endif
