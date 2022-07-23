#include <stdio.h>

int puts(const char* data) {
#ifdef __is_libk
    return tty_puts(data);
#else
    // TODO: implement libc functions with syscalls 'n stuff
#endif
}
