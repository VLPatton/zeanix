#include <stdlib.h>
#include <stdio.h>
 
__attribute__((__noreturn__))
void abort(void) {
#if defined(__is_libk)
	// TODO: Add proper kernel panic.
	printf("KERNEL PANIC: SIGABRT\n");
#else
	// TODO: Abnormally terminate the process as if by SIGABRT.
	printf("abort()\n");
#endif
	while (1);
	__builtin_unreachable();
}