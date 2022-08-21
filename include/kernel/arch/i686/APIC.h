#ifndef _K_ARCH_I686_APIC_H
#define _K_ARCH_I686_APIC_H

/*
    This header includes methods for both the APIC and the IOAPIC(s)
*/

#include <arch/i686/ACPI.h>
#include <arch/i686/CPUID.h>

#define _IA32_APIC_BASE_MSR 0x1B
#define _IA32_APIC_BASE_MSR_BSP 0x100            // Processor is a BSP
#define _IA32_APIC_BASE_MSR_ENABLE 0x800

void* apic_getbase();
void apic_setbase(void*);
void apic_init();
void apic_write(uint32_t, uint32_t);
uint32_t apic_read(uint32_t);
void ioapic_init();
uint32_t ioapic_read(uint32_t);
void ioapic_write(uint32_t, uint32_t);

#endif