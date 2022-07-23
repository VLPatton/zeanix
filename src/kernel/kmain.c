#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <arch/i686/CPUID.h>
#include <arch/i686/paging.h>
#include <arch/i686/multiboot.h>
#include <arch/i686/descriptortables.h>
#include <arch/i686/ps2.h>
#include <serial.h>
#include <initrd.h>

void kmain(void) {
    asm volatile ("cli");
    gdt_init();
    idt_init();

    assert(!serial_init());
    tty_init();
    printf("Zeanix\n\n");

    multiboot_getHeader();                                      // Retrieve the Multiboot header

    initrd_getIndex(module_getIndex(0).module_start);           // Init the initrd system based on Multiboot header

    printf("%s\n", (char*)initrd_getbuffer("etc/os-release"));  // Example: retrieve buffer of initrd file, and print contents to TTY

    paging_init();

    kbd_init();

    for (;;);
    return;
}
