#ifdef __K_DEBUG

#ifndef _K_DEBUG_H
#define _K_DEBUG_H 1

int getDebugChar();     // Reading char from GDB
void putDebugChar(int);  // Sending char to GDB
void exceptionHandler(int, void*);

#endif

#endif