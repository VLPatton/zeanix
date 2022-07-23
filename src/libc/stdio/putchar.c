#include <stdio.h>

#ifdef __K_DEBUG
#include <debug.h>
#endif

int putchar(int ic) {
#ifdef __is_libk
    char c = (char)ic;
    tty_putchar(c);
#ifdef __K_DEBUG
    putDebugChar(ic);
#endif

#else
    // TODO: implement libc functions with syscalls 'n stuff
#endif
    return ic;
}
