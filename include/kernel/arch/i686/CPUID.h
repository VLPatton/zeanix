#ifndef _K_ARCH_I686_CPUID_H
#define _K_ARCH_I686_CPUID_H 1

#include <cpuid.h>
#include <stdint.h>

/* Vendor signatures from manufacturers */
#define CPUID_VENDOR_AMD   0x68747541
#define CPUID_VENDOR_INTEL 0x756e6547

/* C wrappers and functions */
uint8_t inb(uint16_t);                          // C wrapper for asm inb, args: (port)
void outb(uint16_t, uint8_t);                   // C wrapper for asm outb, args: (port, data)
void iowait(void);                              // Wait for approx 1ms
void getmsr(uint32_t, uint32_t*, uint32_t*);    // Read a value from an MSR, args: (msr, *lo, *hi)
void setmsr(uint32_t, uint32_t, uint32_t);      // Write a value to an MSR, args: (msr, lo, hi)

#endif
