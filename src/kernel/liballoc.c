#include <liballoc.h>

#define MAXCOMPLETE     5
#define MAXEXP          32
#define MINEXP          8

#define MODE_BEST       0
#define MODE_INSTANT    1

#define MODE            MODE_BEST

#ifdef __K_DEBUG
#include <stdio.h>
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif

struct boundary_tag*    l_freePages[MAXEXP];
int                     l_completePages[MAXEXP];

#ifdef __K_DEBUG
unsigned int l_allocated = 0;       // The real amount of memory allocated.
unsigned int l_inuse = 0;           // The amount of memory in use (malloc'ed). 
#endif


static int l_initialized = 0;       // Flag to indicate initialization.	
static int l_pageSize  = 4096;      // Individual page size
static int l_pageCount = 16;        // Minimum number of pages to allocate.

/* Helper Functions */

// Disables interrupts and "locks" memory structures
// Returns 0 on success, -1 if failure
int liballoc_lock() {
    asm volatile ("cli");
    return 0;
}

int liballoc_unlock() {
    asm volatile ("sti");
    return 0;
}

// Returns the exponent required to manage 'size' amount of memory
// Returns n where 2^n <= size < 2^(n+1)
static inline int getexp(uint32_t size)
{
	if (size < (1<<MINEXP)) {
		#ifdef __K_DEBUG
		printf("getexp returns -1 for %i less than MINEXP\n", size);
		#endif
		return -1;	// Smaller than the quantum.
	}
	
	uint32_t shift = MINEXP;

	while (shift < MAXEXP) {
		if ((uint32_t)(1 << shift) > size) break;
		shift += 1;
	}

	#ifdef __K_DEBUG
	printf("getexp returns %i (%i bytes) for %i size\n", shift - 1, (1<<(shift -1)), size);
	#endif

	return shift - 1;	
}

// memset() designed specifically for liballoc
// Returns (void*)s with the (char*)s filled with (char)c
static void* liballoc_memset(void* s, int c, size_t n) {
    size_t i;
    for (i = 0; i < n; i++)
        ((char*)s)[i] = (char)c;
    return s;
}

// memcpy() designed specifically for liballoc
// Returns pointer to destination, (void*)s1
static void* liballoc_memcpy(void* s1, const void* s2, size_t n) {
    char *cdest;
    char *csrc;
    unsigned int *ldest = (unsigned int*)s1;
    unsigned int *lsrc  = (unsigned int*)s2;

    while (n >= sizeof(unsigned int)) {
        *ldest++ = *lsrc++;
        n -= sizeof(unsigned int);
    }

    cdest = (char*)ldest;
    csrc  = (char*)lsrc;

    while (n > 0) {
        *cdest++ = *csrc++;
        n -= 1;
    }

    return s1;
}

// Dumps information to stdout for debug
// Returns nothing
#ifdef __K_DEBUG
static void dump_array() {
	int i = 0;
	struct boundary_tag *tag = NULL;

	printf("------ Free pages array ---------\n");
	printf("System memory allocated: %d\n", l_allocated );
	printf("Memory in used (malloc'ed): %d\n", l_inuse );

		for ( i = 0; i < MAXEXP; i++ )
		{
			printf("%.2i(%i): ",i, l_completePages[i] );
	
			tag = l_freePages[ i ];
			while (tag != NULL)
			{
				if (tag->split_left  != NULL) printf("*");
				printf("%d", tag->real_size );
				if (tag->split_right != NULL) printf("*");
	
				printf(" ");
				tag = tag->next;
			}
			printf("\n");
		}

	printf("'*' denotes a split to the left/right of a tag\n");
	//flush(stdout);  // unnecessary fflush since there's no standard file descriptor for stdout in the libk yet
}
#endif

// 
//
static inline void insert_tag(struct boundary_tag* tag, int index) {
	int realIndex;
	
	if (index < 0) 
	{
		realIndex = getexp(tag->real_size - sizeof(struct boundary_tag));
		if (realIndex < MINEXP) realIndex = MINEXP;
	}
	else
		realIndex = index;
	
	tag->index = realIndex;
	
	if (l_freePages[realIndex] != NULL) 
	{
		l_freePages[realIndex]->prev = tag;
		tag->next = l_freePages[realIndex];
	}

	l_freePages[realIndex] = tag;
}

// Removes a tag from the indices of free pages
// Returns nothing
static inline void remove_tag(struct boundary_tag* tag) {
	if (l_freePages[tag->index] == tag) l_freePages[tag->index] = tag->next;

	if (tag->prev != NULL) tag->prev->next = tag->next;
	if (tag->next != NULL) tag->next->prev = tag->prev;

	tag->next = NULL;
	tag->prev = NULL;
	tag->index = -1;
}

// Combines the tag->split_left and tag structs
// Returns a merged (struct boundary_tag*) of tag and tag->split_left
static inline struct boundary_tag* melt_left(struct boundary_tag* tag) {
	struct boundary_tag *left = tag->split_left;
							
	left->real_size   += tag->real_size;
	left->split_right  = tag->split_right;
	
	if (tag->split_right != NULL) tag->split_right->split_left = left;

	return left;
}

// Combines the tag->split_right and tag structs
// Returns the original tag, modified to include the right tag's space
static inline struct boundary_tag* absorb_right(struct boundary_tag* tag)
{
    struct boundary_tag *right = tag->split_right;

    remove_tag(right);		// Remove right from free pages.

    tag->real_size   += right->real_size;

    tag->split_right  = right->split_right;
    if ( right->split_right != NULL )
        right->split_right->split_left = tag;

    return tag;
}

// Split a tag (?)
// Returns a new stag struct created by splitting a tag
static inline struct boundary_tag* split_tag(struct boundary_tag* tag) {
    unsigned int remainder = tag->real_size - sizeof(struct boundary_tag) - tag->size;
        
    struct boundary_tag *new_tag = 
        (struct boundary_tag*)((unsigned int)tag + sizeof(struct boundary_tag) + tag->size);	

    new_tag->magic = LIBALLOC_MAGIC;
    new_tag->real_size = remainder;	

    new_tag->next = NULL;
    new_tag->prev = NULL;

    new_tag->split_left = tag;
    new_tag->split_right = tag->split_right;

    if (new_tag->split_right != NULL) new_tag->split_right->split_left = new_tag;
    tag->split_right = new_tag;

    tag->real_size -= new_tag->real_size;

    insert_tag(new_tag, -1);

    return new_tag;
}

/* End Helper Functions */

// Finds an empty tag to allocate
//
static struct boundary_tag* allocate_new_tag(unsigned int size)
{
    unsigned int pages;
    unsigned int usage;
    struct boundary_tag *tag;

    // This is how much space is required.
    usage  = size + sizeof(struct boundary_tag);

    // Perfect amount of space
    pages = usage / l_pageSize;
    if ((usage % l_pageSize) != 0) pages += 1;

    // Make sure it's >= the minimum size.
    if (pages < (uint32_t)l_pageCount) pages = l_pageCount;

    tag = (struct boundary_tag*)liballoc_alloc(pages);

    if (tag == NULL) return NULL;	// uh oh, we ran out of memory.
    
            tag->magic 		= LIBALLOC_MAGIC;
            tag->size 		= size;
            tag->real_size 	= pages * l_pageSize;
            tag->index 		= -1;

            tag->next		= NULL;
            tag->prev		= NULL;
            tag->split_left 	= NULL;
            tag->split_right 	= NULL;


    #ifdef DEBUG
    printf("Resource allocated %x of %d pages (%d bytes) for %d size.\n", tag, pages, pages * l_pageSize, size );

    l_allocated += pages * l_pageSize;

    printf("Total memory usage = %d KB\n",  (int)((l_allocated / (1024))) );
    #endif
    
    return tag;
}

