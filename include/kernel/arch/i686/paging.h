#ifndef _K_ARCH_I686_PAGING_H
#define _K_ARCH_I686_PAGING_H 1

#include <stddef.h>
#include <stdint.h>

#define _SUPERUSER_PAGE_FLAGS 0x009B
#define _USERKERNL_PAGE_FLAGS 0x009D
#define _USERSPACE_PAGE_FLAGS 0x009E

#define PAGING_IDX_FROM_BIT(a) (a / (8 * 4))
#define PAGING_OFF_FROM_BIT(a) (a % (8 * 4))

typedef struct page {
    uint32_t present    : 1;    // Page present in memory
    uint32_t rw         : 1;    // Read-only if clear, readwrite if set
    uint32_t user       : 1;    // Supervisor level only if clear
    uint32_t accessed   : 1;    // Has the page been accessed since last refresh?
    uint32_t dirty      : 1;    // Has the page been written to since last refresh?
    uint32_t unused     : 7;    // Amalgamation of unused and reserved bits
    uint32_t frame      : 20;   // Frame address (shifted right 12 bits)
} page_t;

typedef struct page_table {
   page_t pages[1024];
} page_table_t;

typedef struct page_dir {
    /*
        Array of pointers to the pagetables above, but gives their *physical*
        location, for loading into the CR3 register.
    */
    page_table_t* tables[1024];

    page_table_t** (*tablesPhysical);
    /*
        The physical address of tablesPhysical. This comes into play
        when we get our kernel heap allocated and the directory
        may be in a different location in virtual memory.
    */
    uint32_t physicalAddr;
} page_dir_t;

void paging_init();
void paging_switchdir();
page_t* paging_getpage();

#endif
