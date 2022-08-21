#include <initrd.h>
#include <string.h>

initrd_tarFileHeader_t* initrd_headers[1024];
uint32_t initrd_maxIndex;

// Determine value based on octal string
size_t otob(const char* in) {
    size_t size = 0;
    size_t j;
    size_t count = 1;

    for (j = 11; j > 0; j--, count *= 8)
        size += ((in[j - 1] - '0') * count);
 
    return size;
}

// Find the maximum index of files and initialize the initrd system
uint32_t initrd_getIndex(void* location) {
    initrd_tarFileHeader_t* header = (initrd_tarFileHeader_t*)location;
    uint32_t i;
    // Search through all valid headers and break when an invalid one is found
    for (i = 0; (*header).filename[0] != '\0'; i++) {
        // Make sure variable "header" is accurate
        header = (initrd_tarFileHeader_t*)location;

        // Determine the size of the file
        size_t size = otob((*header).size);

        // Store the pointer to the header in the pointer array "initrd_headers"
        initrd_headers[i] = header;

        // Move to next header
        location += ((size / 512) + 1) * 512;
        if (size % 512)
            location += 512;
    }
    initrd_maxIndex = i;
    return i;
}

// Get filename from index
char* initrd_getFFI(uint32_t index) {
    // Return the char buffer "filename" from the header struct referenced by initrd_headers[index]
    return (*initrd_headers[index]).filename;
}

// Get index from filename
uint32_t initrd_getIFF(const char* name) {
    uint32_t index;
    for (index = 0; index < initrd_maxIndex; index++)
        if (!memcmp(name, (*initrd_headers[index]).filename, strlen(name)))
            return index;
    return initrd_maxIndex;
}

// Return the buffer of an associated file name - NULL if file not found
void* initrd_getbuffer(const char* name) {
    uint32_t index = initrd_getIFF(name);
    if (*(*initrd_headers[index]).typeflag == '0' || *(*initrd_headers[index]).typeflag == '\0') {
        return (void*)((char*)initrd_headers[index] + 512);
    }
    return (void*)0;
}
