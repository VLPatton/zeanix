#include <arch/i686/descriptortables.h>
#ifdef __K_DEBUG
#include <stdio.h>
#endif

gdt_entry_t gdt_entries[6];
gdt_ptr_t   gdt;
tss_t tss;

void gdt_setGate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt_entries[num].base_low       = (base & 0xFFFF);
    gdt_entries[num].base_mid       = (base >> 16) & 0xFF;
    gdt_entries[num].base_hi        = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low      = (limit & 0xFFFF);
    gdt_entries[num].gran           = (limit >> 16) & 0x0F;

    gdt_entries[num].gran           |= gran & 0xF0;
    gdt_entries[num].access         =  access;
}

void gdt_init() {
    gdt.limit = (sizeof(gdt_entry_t) * 6) - 1;
    gdt.base  = (uint32_t)&gdt_entries;

    gdt_setGate(0, 0, 0, 0, 0);                 // Null segment
    gdt_setGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);  // Code segment
    gdt_setGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);  // Data segment
    gdt_setGate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);  // User mode code segment
    gdt_setGate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);  // User mode data segment

    // Set TSS
    asm volatile (
        "mov %0, esp"
        : "=g" (tss.esp0)
    );
    tss.ss0 = 0x10;
    tss.iopb = 104;
    gdt_setGate(5, (uint32_t)&tss, 104, 0x89, 0x00);

    gdt_flush(&gdt);
    tss_flush();
}


