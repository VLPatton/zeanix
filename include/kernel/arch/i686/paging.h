#ifndef _K_ARCH_I686_PAGING_H
#define _K_ARCH_I686_PAGING_H 1

#include <stddef.h>
#include <stdint.h>

void paging_init();
void paging_map(void*, void*);

#endif
