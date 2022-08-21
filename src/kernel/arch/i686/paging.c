#include <arch/i686/paging.h>
#include <liballoc.h>
#include <serial.h>
#include <assert.h>
#include <string.h>

#ifdef __K_DEBUG
#include <stdio.h>
#endif

#define PAGE_SIZE 4096

// A bitmap of frames - used or free.
uint32_t frames[1024 * 8];  // Could be up to 1024 x 1024 for a full page map of 4GiB
uint32_t nframes;

// Static function to set a bit in the frames bitset
static void set_frame(uint32_t frame_addr) {
    uint32_t frame = frame_addr / 0x1000;
    uint32_t idx = PAGING_IDX_FROM_BIT(frame);
    uint32_t off = PAGING_OFF_FROM_BIT(frame);
    frames[idx] |= (0x1 << off);
}

// Static function to clear a bit in the frames bitset
static void clear_frame(uint32_t frame_addr) {
    uint32_t frame = frame_addr / 0x1000;
    uint32_t idx = PAGING_IDX_FROM_BIT(frame);
    uint32_t off = PAGING_OFF_FROM_BIT(frame);
    frames[idx] &= ~(0x1 << off);
}

// Static function to test if a bit is set.
static uint32_t test_frame(uint32_t frame_addr) {
    uint32_t frame = frame_addr / 0x1000;
    uint32_t idx = PAGING_IDX_FROM_BIT(frame);
    uint32_t off = PAGING_OFF_FROM_BIT(frame);
    return (frames[idx] & (0x1 << off));
}

// Static function to find the first free frame.
static uint32_t first_frame() {
    uint32_t i, j;
    for (i = 0; i < PAGING_IDX_FROM_BIT(nframes); i++) {
        if (frames[i] != 0xFFFFFFFF) {
            // at least one bit is free here.
            for (j = 0; j < 32; j++) {
                uint32_t toTest = 0x1 << j;
                if (!(frames[i] & toTest))
                    return i * 4 * 8 + j;
            }
        }
    }
    return 0;
} 

// Function to allocate a frame.
void alloc_frame(page_t* page, int kernel, int rw) {
    if (page->frame != 0) {
        return; // Frame was already allocated, return straight away.
    } else {
        uint32_t idx = first_frame();   // idx is now the index of the first free frame.
        assert(idx != (uint32_t)-1);
        set_frame(idx * 0x1000);        // Frame now marked
        page->present = 1;              // Mark it as present.
        page->rw = (rw) ? 1 : 0;        // Should the page be writeable?
        page->user = (kernel) ? 0 : 1;  // Should the page be user-mode?
        page->frame = idx;
    }
}

void free_frame(page_t* page) {
    if (page->present == 0) {
        return;
    } else {
        clear_frame(page->frame << 12);
        page->present = 0;
        page->rw = 0;
        page->user = 0;
        page->frame = 0;
    }
}

/* End Help Functions */

extern page_table_t** pagetable;
page_dir_t pagedir;

void paging_enable() {
    asm volatile (
        "mov cr3, eax"
        : : "a" (*pagedir.tablesPhysical)
    );
    asm volatile (
        "mov eax, cr0\n"
        "or  eax, 0x80000000\n"
        "mov cr0, eax"
    );
}

void paging_init() {
    uint32_t end_page = 0x8000000;

    nframes = end_page / 0x1000;
    memset(frames, 0, PAGING_IDX_FROM_BIT(nframes));

    memset(&pagedir, 0, sizeof(page_dir_t));
    pagedir.tablesPhysical = &pagetable;

    for (int i = 0x0; i < 0x40000; i += 0x1000) {
        alloc_frame(paging_getpage(i), 1, 1);
    }

    paging_enable();
}

page_t* paging_getpage(uint32_t addr) {
    addr /= 0x1000;

    uint32_t table_idx = addr / 1024;
    if (pagedir.tables[table_idx]) {
        return &pagedir.tables[table_idx]->pages[addr % 1024];
    } else {
        uint32_t tmp = addr * 0x1000;
        pagedir.tables[table_idx] = (page_table_t*)tmp;
        memset(pagedir.tables[table_idx], 0, 0x1000);
        (*pagedir.tablesPhysical)[table_idx] = (page_table_t*)(tmp | 0x7); // PRESENT, RW, US.
        return &pagedir.tables[table_idx]->pages[addr % 1024];
    }
}

/* liballoc functions */

void* liballoc_alloc(int pages) {
    void* ptr = NULL;



    return ptr;
}

int liballoc_free(void* ptr, int pages) {
    uint32_t frame = (uint32_t)ptr / 0x1000;

    return 0;
}
