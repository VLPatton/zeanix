#ifndef _K_LIBALLOC_H
#define _K_LIBALLOC_H 1

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// Liballoc compatible macros
#define LIBALLOC_MAGIC 0xC001C0DE

struct boundary_tag {
    uint32_t    magic;                  // Magic number (0xC001C0DE)
    uint32_t    size;                   // Requested size
    uint32_t    real_size;              // Actual size
    int32_t     index;                  // Location in the page table

    struct boundary_tag* split_left;    // Linked list info for broken pages
    struct boundary_tag* split_right;   // Linked list info for broken pages

    struct boundary_tag *next;          // Linked list info.
    struct boundary_tag *prev;          // Linked list info.
};

// Initializes the liballoc variables, such as page size
// Returns 0 on success, -1 if failure
int liballoc_init();

// Disables interrupts and "locks" memory structures
// Returns 0 on success, -1 if failure
int     liballoc_lock();

// Re-enables interrupts and "unlocks" memory structures
// Returns 0 on success, -1 if failure
int     liballoc_unlock();

// Uses paging allocator to allocate memory to liballoc
// Returns a pointer to alloc'd memory on success, NULL on failure
void*   liballoc_alloc(int);

// Frees pages allocated by liballoc_alloc()
// Returns 0 on success, -1 if failure
int     liballoc_free(void*, int);

void*   malloc(size_t);                 // The standard function.
void*   realloc(void*, size_t);         // The standard function.
void*   calloc(size_t, size_t);         // The standard function.
void    free(void*);                    // The standard function.


#ifdef __cplusplus
}
#endif

#endif
