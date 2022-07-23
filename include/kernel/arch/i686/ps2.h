#ifndef _K_ARCH_I686_H
#define _K_ARCH_I686_H 1

#include <stdint.h>
#include <arch/i686/interrupts.h>

int kbd_init();    // NOTE: this keyboard IF uses USB Legacy Support for emulating PS/2,
                    // there should be much more done here for a full keyboard IF
void kbd_callback(const registers_t*);

#endif
