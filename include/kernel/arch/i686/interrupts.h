#ifndef _K_ARCH_I686_INTERRUPTS_H
#define _K_ARCH_I686_INTERRUPTS_H

#include <stdint.h>

#define IRQ_0 32
#define IRQ_1 33

// Interrupt Register Context
typedef struct registers {
    uint32_t ds;
    uint32_t edi, esi, ebp, unused, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, esp, ss;
} registers_t; 

typedef void (*isr_t)(const registers_t*);

void int_registerHandler(uint8_t, isr_t);

#endif
