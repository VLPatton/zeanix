#ifndef _STRING_H
#define _STRING_H 1
 
#include <sys/cdefs.h>
 
#include <stddef.h>
 
#ifdef __cplusplus
extern "C" {
#endif
 
int memcmp(const void*, const void*, size_t);                   // MEM CoMPare
void* memcpy(void* __restrict, const void* __restrict, size_t); // MEM CoPY
void* memmove(void*, const void*, size_t);                      // MEM MOVE
void* memset(void*, int, size_t);                               // MEM SET
size_t strlen(const char*);                                     // STRing LENgth
void strrev(char*);                                             // STRing REVerse

#ifdef __cplusplus
}
#endif
 
#endif
