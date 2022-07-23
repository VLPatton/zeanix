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
} multiboot_header_t;

typedef struct module_header {
    void* module_start;
    void* module_end;
    char* cmdline;
    uint32_t reserved;
} module_header_t;

#pragma pack(0)

multiboot_header_t multiboot_getHeader();
module_header_t module_getIndex(uint32_t);

#endif
