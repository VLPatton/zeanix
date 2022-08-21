#include <arch/i686/descriptortables.h>
#include <string.h>
#include <arch/i686/CPUID.h>
#include <arch/i686/APIC.h>

idt_entry_t idt_entries[256];   // Entries pointed to by idtr
idt_ptr_t   idt;                // IDT pointer struct, gets loaded into idtr

extern void isr0 ();            // Except. #DE
extern void isr1 ();            // Except. #DB
extern void isr2 ();            // Intrpt. NMI
extern void isr3 ();            // Except. #BP
extern void isr4 ();            // Except. #OF
extern void isr5 ();            // Except. #BR
extern void isr6 ();            // Except. #UD
extern void isr7 ();            // Except. #NM
extern void isr8 ();            // Except. 
extern void isr9 ();            // Except. 
extern void isr10();            // Except. 
extern void isr11();            // Except. 
extern void isr12();            // Except. 
extern void isr13();            // Except. 
extern void isr14();            // Except. 
extern void isr15();            // Except. 
extern void isr16();            // Except. 
extern void isr17();            // Except. 
extern void isr18();            // Except. 
extern void isr19();            // Except. 
extern void isr20();            // Except. 
extern void isr21();            // Except. 
extern void isr22();            // Except. 
extern void isr23();            // Except. 
extern void isr24();            // Except. 
extern void isr25();            // Except. 
extern void isr26();            // Except. 
extern void isr27();            // Except. 
extern void isr28();            // Except. 
extern void isr29();            // Except. 
extern void isr30();            // Except. 
extern void isr31();            // Except. 

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
    // Set PIC masks to disable all PIC interrupts (we are opting for APIC instead)
    outb(0x21, 0xFF);
    outb(0xA1, 0xFF);

    // Initialize the APIC automatically
    apic_init();

    // Set limit to sizeof each entry, times how many entries, minus one,
    // and set base to the address of the array of entries
    idt.limit = sizeof(idt_entry_t) * 256 - 1;
    idt.base = (uint32_t)&idt_entries;

    // Make sure we have control over the data we're giving to the IDTR and don't
    // give it bogus info that doesn't exist b/c we didn't init properly
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

    // Call an IDT flushing routine
    idt_flush(&idt);
}
