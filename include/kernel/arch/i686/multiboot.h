#ifndef _K_ARCH_I686_MULTIBOOT_H
#define _K_ARCH_I686_MULTIBOOT_H 1

#include <stdint.h>

extern void* multiboot_ptr;

#pragma pack(4)

typedef struct multiboot_header {
    uint32_t flags;

    uint32_t mem_lower;
    uint32_t mem_upper;

    uint32_t bootdev;

    char* cmdline;

    uint32_t module_count;
    void* module_addr;

    char symbols[12];

    uint32_t mmap_length;
    void* mmap_addr;
} multiboot_header_t;

typedef struct module_header {
    void* module_start;
    void* module_end;
    char* cmdline;
    uint32_t reserved;
} module_header_t;

typedef struct mmap_section {
    uint64_t base_addr; // Upper 4 bytes reserved on i686
    uint64_t length;    // ^
    uint32_t type;      // 0 Reserved, 1 RAM, 2 Reserved, 3 ACPI, 4 Reserved, 5 Defective
} mmap_section_t;

#pragma pack(0)

multiboot_header_t multiboot_getHeader();   // Find and return the information struct of the Multiboot header 
module_header_t module_getIndex(uint32_t);  // Return an information struct on a module based on index
mmap_section_t mmap_getmap(uint32_t);       // Return an information struct on a mmap section based on index
uint32_t mmap_getBufferLen();               // Return the length of the memory map in bytes (multiboot_header_t.mmap_length)

#endif
