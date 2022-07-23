#include <arch/i686/paging.h>

/*
    Heap will be encompassed in one 4MiB page
    Paging will begin at 4MiB physical
*/

uint32_t*    pagedir;
void*        pagedir_location;
uint32_t*    pagedir_last;

void paging_map(void* virt, void* phys) {
    uint16_t id = (uint32_t)virt >> 22;
    for(int i = 0; i < 1024; i++)
    {
        pagedir_last[i] = (uint32_t)phys | 3;
        phys += 4096;
    }
    pagedir[id] = ((uint32_t)pagedir_last) | 3;
    pagedir_last = (uint32_t*)(((uint32_t)pagedir_last) + 4096);
}

void paging_enable() {
    asm volatile (
        "mov cr3, eax"
        : : "a" (pagedir_location)
    );
    asm volatile (
        "mov eax, cr0\n"
        "or  eax, 0x80000000\n"
        "mov cr0, eax"
    );
}

void paging_init() {
    pagedir = (uint32_t*)0x400000;
    pagedir_location = (void*)(uint32_t)pagedir;
    pagedir_last = (uint32_t*)0x1000000; // Four 4MiB page dirs
    for(int i = 0; i < 1024; i++) {
        pagedir[i] = 0 | 2;
    }
    paging_map((void*)0, (void*)0);
    paging_map((void*)0x400000, (void*)0x400000);
    paging_enable();
}
