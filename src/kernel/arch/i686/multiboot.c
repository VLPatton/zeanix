#include <arch/i686/multiboot.h>

static multiboot_header_t header_final;

multiboot_header_t multiboot_getHeader() {
    multiboot_header_t* header = (multiboot_header_t*)multiboot_ptr;
    header_final = *header;
    return *header;
}

module_header_t module_getIndex(uint32_t index) {
    module_header_t* header_ptr = (module_header_t*)header_final.module_addr;
    return header_ptr[index];
}
