#include <arch/i686/descriptortables.h>
#include <string.h>
#include <arch/i686/CPUID.h>

idt_entry_t idt_entries[256];
idt_ptr_t   idt;

extern void isr0 ();
extern void isr1 ();
extern void isr2 ();
extern void isr3 ();
extern void isr4 ();
extern void isr5 ();
extern void isr6 ();
extern void isr7 ();
extern void isr8 ();
extern void isr9 ();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void irq0 ();
extern void irq1 ();
extern void irq2 ();
extern void irq3 ();
extern void irq4 ();
extern void irq5 ();
extern void irq6 ();
extern void irq7 ();
extern void irq8 ();
extern void irq9 ();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

extern void irq0x69();  // Used for syscalls; actually closer to an ISR than IRQ 
                        // since it is actually on INT 0x69, not 0x89

void idt_setGate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt_entries[num].base_low   = base & 0xFFFF;
    idt_entries[num].base_hi    = (base >> 16) & 0xFFFF;

    idt_entries[num].selector   = sel;
    idt_entries[num].reserved   = 0;
    // OR'd with 0x60 for Ring 3 descriptors
    idt_entries[num].flags      = flags /*| 0x60*/;
} 

void idt_init() {
    // Remap PICs
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    // Set PIC masks (0x00, for all available)
    outb(0x21, 0xFD);
    outb(0xA1, 0xFF);

    idt.limit = sizeof(idt_entry_t) * 256 - 1;
    idt.base = (uint32_t)&idt_entries;

    memset(&idt_entries, 0, sizeof(idt_entry_t) * 256);

    // Set ISRs in the IDT
    idt_setGate(0 , (uint32_t)isr0 , 0x08, 0x8F);
    idt_setGate(1 , (uint32_t)isr1 , 0x08, 0x8F);
    idt_setGate(2 , (uint32_t)isr2 , 0x08, 0x8E);
    idt_setGate(3 , (uint32_t)isr3 , 0x08, 0x8F);
    idt_setGate(4 , (uint32_t)isr4 , 0x08, 0x8F);
    idt_setGate(5 , (uint32_t)isr5 , 0x08, 0x8F);
    idt_setGate(6 , (uint32_t)isr6 , 0x08, 0x8F);
    idt_setGate(7 , (uint32_t)isr7 , 0x08, 0x8F);
    idt_setGate(8 , (uint32_t)isr8 , 0x08, 0x8F);
    idt_setGate(9 , (uint32_t)isr9 , 0x08, 0x8F);
    idt_setGate(10, (uint32_t)isr10, 0x08, 0x8F);
    idt_setGate(11, (uint32_t)isr11, 0x08, 0x8F);
    idt_setGate(12, (uint32_t)isr12, 0x08, 0x8F);
    idt_setGate(13, (uint32_t)isr13, 0x08, 0x8F);
    idt_setGate(14, (uint32_t)isr14, 0x08, 0x8F);
    idt_setGate(15, (uint32_t)isr15, 0x08, 0x8F);
    idt_setGate(16, (uint32_t)isr16, 0x08, 0x8F);
    idt_setGate(17, (uint32_t)isr17, 0x08, 0x8F);
    idt_setGate(18, (uint32_t)isr18, 0x08, 0x8F);
    idt_setGate(19, (uint32_t)isr19, 0x08, 0x8F);
    idt_setGate(20, (uint32_t)isr20, 0x08, 0x8F);
    idt_setGate(21, (uint32_t)isr21, 0x08, 0x8F);
    idt_setGate(22, (uint32_t)isr22, 0x08, 0x8F);
    idt_setGate(23, (uint32_t)isr23, 0x08, 0x8F);
    idt_setGate(24, (uint32_t)isr24, 0x08, 0x8F);
    idt_setGate(25, (uint32_t)isr25, 0x08, 0x8F);
    idt_setGate(26, (uint32_t)isr26, 0x08, 0x8F);
    idt_setGate(27, (uint32_t)isr27, 0x08, 0x8F);
    idt_setGate(28, (uint32_t)isr28, 0x08, 0x8F);
    idt_setGate(29, (uint32_t)isr29, 0x08, 0x8F);
    idt_setGate(30, (uint32_t)isr30, 0x08, 0x8F);
    idt_setGate(31, (uint32_t)isr31, 0x08, 0x8F);

    // Set IRQs and IRQ0x69 in IDT
    idt_setGate(32, (uint32_t)irq0 , 0x08, 0x8E);
    idt_setGate(33, (uint32_t)irq1 , 0x08, 0x8E);
    idt_setGate(34, (uint32_t)irq2 , 0x08, 0x8E);
    idt_setGate(35, (uint32_t)irq3 , 0x08, 0x8E);
    idt_setGate(36, (uint32_t)irq4 , 0x08, 0x8E);
    idt_setGate(37, (uint32_t)irq5 , 0x08, 0x8E);
    idt_setGate(38, (uint32_t)irq6 , 0x08, 0x8E);
    idt_setGate(39, (uint32_t)irq7 , 0x08, 0x8E);
    idt_setGate(40, (uint32_t)irq8 , 0x08, 0x8E);
    idt_setGate(41, (uint32_t)irq9 , 0x08, 0x8E);
    idt_setGate(42, (uint32_t)irq10, 0x08, 0x8E);
    idt_setGate(43, (uint32_t)irq11, 0x08, 0x8E);
    idt_setGate(44, (uint32_t)irq12, 0x08, 0x8E);
    idt_setGate(45, (uint32_t)irq13, 0x08, 0x8E);
    idt_setGate(46, (uint32_t)irq14, 0x08, 0x8E);
    idt_setGate(47, (uint32_t)irq15, 0x08, 0x8E);
    idt_setGate(0x69, (uint32_t)irq0x69, 0x08, 0x8E);

    idt_flush(&idt);
}
