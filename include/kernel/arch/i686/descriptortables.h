#ifndef _K_ARCH_I686_DESCRIPTORTABLES_H
#define _K_ARCH_I686_DESCRIPTORTABLES_H 1

#include <stddef.h>
#include <stdint.h>

#pragma pack(1)

/// GDT SECTION ///

typedef struct gdt_entry {
    uint16_t    limit_low;  // Lower 16 bits of limit
    uint16_t    base_low;   // Lowest 16 bits of base
    uint8_t     base_mid;   // Middle 8 bits of base
    uint8_t     access;     // Access flags (P, R, R, D, E, C, W, A) (see below)
    uint8_t     gran;       // Granularity (G, D, 0, 0, L, L, L, L) (see below)
    uint8_t     base_hi;
} gdt_entry_t;
/*
    Access byte:
    P - Present = 1
    R - Ring level = {0,1,2,3}
    D - Descriptor type = 0 for system segment; 1 for code/data
    E - Executable = 0 for data segment; 1 for code segment
    C - Conforming/direction =  usually 0 for data segment; usually 1 for code segment
    W - read/Write = generally always 1
    A - Accessed = CPU will set on its own, best to leave clear on init

    Granularity byte:
    G - Granularity = 0 for 1B; 1 for 1kiB
    D - operanD size = 0 for 16b; 1 for 32b
    0 - Zero = reserved on i686
    L - "limit_hi" = upper 4 bits of limit
*/

typedef struct _gdt_ptr {
    uint16_t limit; // Size of GDT
    uint32_t base;  // Pointer to first "gdt_entry_t" in the structure
} gdt_ptr_t;

/// TSS SECTION ///

typedef struct _tss {
    uint32_t reserve1;  // Unused
    uint32_t esp0;      // Ring 0 stack
    uint16_t ss0;       // Kernel data segment descriptor
    char reserve2[92];  // Unused
    uint16_t iopb;      // I/O bitmap, unused, stores 104U for size
    uint32_t reserve3;  // Unused
} tss_t;

/// IDT SECTION ///

typedef struct _idt_entry {
    uint16_t base_low;
    uint16_t selector;
    uint8_t  reserved;
    uint8_t  flags;     // (P, R, R, 0, T, T, T, T) (see below)
    uint16_t base_hi;
} idt_entry_t;
/*
    Flags byte:
    P - Present = 1
    R - Ring level = 0 for hardware ints and exceptions, 3 for everything else
    0 - Zero = reserved
    T - gate Type = 0x5 for task, 0xE for 32b Int, 0xF for 32b Trap
*/

typedef struct _idt_ptr {
    uint16_t limit; // Size of the IDT
    uint32_t base;  // Base address of the IDT
} idt_ptr_t;

#pragma pack(0)

extern void gdt_flush(void*);
extern void tss_flush(void);
extern void idt_flush(void*);

void gdt_init();
void idt_init();

#endif
