#include <arch/i686/interrupts.h>
#include <stdio.h>
#include <stdlib.h>

const uint16_t irq_pic1cmd = 0x20;
const uint16_t irq_pic2cmd = 0xA0;
const uint8_t irq_EOI = 0x20;

isr_t int_handlers[256];

void isr_handler(const registers_t* args) {
    uint32_t number = (*args).int_no;
    printf("Exception received: %d!\n", number);
    if (number == 6) {
        printf("EIP: %d\n", args->eip);
    }
    return;
}

void irq_handler(const registers_t* args) {
    printf("Interrupt received: %d\n", args->int_no);
    if (args->int_no >= 40)
        outb(irq_pic2cmd, irq_EOI);
    outb(irq_pic1cmd, irq_EOI);
    if (int_handlers[args->int_no]) {
        isr_t handler = int_handlers[args->int_no];
        handler(args);
    } else {
        printf("INTERRUPT UNHANDLED!\n");
    }
}

void int_registerHandler(uint8_t num, isr_t handler) {
    int_handlers[num] = handler;
}
