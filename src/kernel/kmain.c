#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <arch/i686/CPUID.h>
#include <arch/i686/paging.h>
#include <arch/i686/pmm.h>
#include <arch/i686/multiboot.h>
#include <arch/i686/descriptortables.h>
#include <arch/i686/ps2.h>
#include <arch/i686/paging.h>
#include <serial.h>
#include <initrd.h>

void kmain(void) {
    asm volatile ("cli");
    multiboot boot = multiboot_getHeader();

    init_pmm(&boot);
    paging_init();     

    assert(!serial_init());
    tty_init();
    printf("Zeanix\n\n");
    dump_multiboot_infos(&boot);

    gdt_init();
    //idt_init();
    asm volatile ("sti");
 
    /*
    initrd_getIndex((void*)module_getIndex(0).mod_start);           // Init the initrd system based on Multiboot header

    kbd_init();
    */
    for (;;);
    return;
}
