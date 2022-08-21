#include <arch/i686/CPUID.h>
#include <arch/i686/APIC.h>
#include <arch/i686/ACPI.h>
#include <string.h>

/* General CPU functions */

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

// C wrapper for asm rdmsr
void getmsr(uint32_t msr, uint32_t* lo, uint32_t* hi) {
    asm volatile("rdmsr" : "=a"(*lo), "=d"(*hi) : "c"(msr));
}

// C wrapper for asm wrmsr
void setmsr(uint32_t msr, uint32_t lo, uint32_t hi) {
    asm volatile("wrmsr" : : "a"(lo), "d"(hi), "c"(msr));
}

/* APIC section of CPU-related functions */

// Determine the base of the APIC
void* apic_getbase() {
    // Retrieve 32b pointer address of APIC
    uint32_t eax, edx;
    getmsr(_IA32_APIC_BASE_MSR, &eax, &edx);

    // Return aligned pointer (APIC must be aligned in physical memory space)
    return (void*)(eax & 0xFFFFF000);
}

// Set the base of the APIC (must be properly aligned)
void apic_setbase(void* base) {
    uint32_t edx = 0;
    uint32_t eax = ((uint32_t)base & 0xFFFFF000) | _IA32_APIC_BASE_MSR_ENABLE;

    setmsr(_IA32_APIC_BASE_MSR, eax, edx);
}

// Initialize the APIC and IOAPIC
void apic_init() {
    // Enable LAPIC
    apic_setbase(apic_getbase());

    // Set Spurious Interrupt Vector Register to 0x01FF to begin receiving interrupts
    apic_write(0xF0, 0x01FF);

    // Get and parse MADT

    // Enable IOAPIC Interrupts


}

// Write value to a register in the LAPIC
void apic_write(uint32_t reg, uint32_t data) {
    reg = reg & 0xFFFFFFF0;
    uint32_t volatile* ptr = (uint32_t volatile*)((uint32_t)apic_getbase() + reg);
    *ptr = data;
}

// Read value from a register in the LAPIC
uint32_t apic_read(uint32_t reg) {
    reg = reg & 0xFFFFFFF0;
    uint32_t volatile* ptr = (uint32_t volatile*)((uint32_t)apic_getbase() + reg);
    return *ptr;
}

/* ACPI Section of CPU-related functions */

mmap_section_t acpi_map;

mmap_section_t* acpi_getmem(void) {
    mmap_section_t* map;
    const uint32_t map_len = mmap_getBufferLen() / 20;
    uint32_t i = 0;
    for (; i <= map_len; i++) {
        map = (mmap_section_t*)(uint32_t)(mmap_getmap(i).base_addr);
    }
    if (i != map_len) {
        acpi_map = *map;
        return map;
    }
    return (void*)0;
}

void acpi_init(void) {

}

acpi_rsdtHeader_t* acpi_getmadt(mmap_section_t mapp) {
    if (mapp.type != 3)
        return (acpi_rsdtHeader_t*)0;
    acpi_rsdtHeader_t* temp = (acpi_rsdtHeader_t*)(uint32_t)(mapp.base_addr & 0xFFFFFFFF);
    for (int i = 0; memcmp((*temp).signature, "APIC", 4) != 0; temp += (*temp).length, i++) 
        if (i > 21)
            return (acpi_rsdtHeader_t*)0;
    return temp;
}
