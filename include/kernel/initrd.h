#ifndef _K_INITRD_H
#define _K_INITRD_H 1

#include <stdint.h>
#include <stddef.h>

#pragma pack(1)

// Many of the "numeric" values in the header are encoded as octal into a string
typedef struct initrd_tarFileHeader {
    char filename[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12];
    char chksum[8];
    char typeflag[1];
    char unused[355];
} initrd_tarFileHeader_t;

#pragma pack(0)

uint32_t initrd_getIndex(void*);        // Get the number of present file headers
char* initrd_getFFI(uint32_t);          // Filename From Index
uint32_t initrd_getIFF(const char*);    // Index From Filename
void* initrd_getbuffer(const char*);    // Get file data buffer

#endif
