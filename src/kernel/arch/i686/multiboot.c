#include <arch/i686/multiboot.h>

multiboot_header_t header_final;

multiboot_header_t multiboot_getHeader() {
    if (header_final.flags == 0) {
        multiboot_header_t* header = (multiboot_header_t*)multiboot_ptr;
        header_final = *header;
    }
    return header_final;
}

module_header_t module_getIndex(uint32_t index) {
    module_header_t* header_ptr = (module_header_t*)header_final.module_addr;
    return header_ptr[index];
}

mmap_section_t mmap_getmap(uint32_t index) {
    mmap_section_t* mmap = (mmap_section_t*)header_final.mmap_addr;
    return mmap[index];
}

uint32_t mmap_getBufferLen() {
    return header_final.mmap_length;
}
