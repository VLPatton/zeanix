#ifndef _K_ARCH_I686_ACPI_H
#define _K_ARCH_I686_ACPI_H

/*
    THIS IS AN INCOMPLETE IMPLEMENTATION OF ACPI USED SOLELY FOR FINDING 
    THE IOAPIC. IT IS ON THE TODO LIST TO IMPLEMENT FURTHER.
*/

#include <arch/i686/multiboot.h>

#pragma pack(1)

typedef struct acpi_rsdtHeader {
    char signature[4];
    uint32_t length;
    uint8_t rev;
    uint8_t chksum;
    char oem_id[6];
    char oem_tableid[8];
    uint32_t oem_rev;
    uint32_t creator_id;
    uint32_t creator_rev;
} acpi_rsdtHeader_t;

typedef struct madt_entryheader {
    uint8_t type;
    uint8_t length;
} madt_entryheader_t;

#pragma pack(0)

mmap_t* acpi_getmem(void);
void acpi_init(void);
acpi_rsdtHeader_t* acpi_getmadt(mmap_t);

#endif
