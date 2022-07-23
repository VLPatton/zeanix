#include <arch/i686/ps2.h>
#include <arch/i686/CPUID.h>

const uint16_t ps2_dataPort = 0x60;
const uint16_t ps2_statusRegister = 0x64;
const uint16_t ps2_cmdRegister = 0x64;
const uint8_t ps2_ack = 0xFA;

int kbd_init() {
    // Init local variable for detecting acknowledges from PS/2 controller
    uint8_t ack;

    // Disable input until init is finished
    outb(ps2_cmdRegister, 0xA7);
    outb(ps2_cmdRegister, 0xAD);
    
    // Flush output buffer of PS/2 controller
    while (inb(ps2_statusRegister) & 0x01)
        inb(ps2_dataPort);
    
    // Set controller config byte
    outb(ps2_cmdRegister, 0x20);
    uint8_t config = (uint8_t)inb(ps2_dataPort);
    config &= 0xBF;
    outb(ps2_cmdRegister, 0x60);
    outb(ps2_dataPort, config);

    // Enable and reset devices on PS/2 controller
    outb(ps2_cmdRegister, 0xA8);
    outb(ps2_cmdRegister, 0xAE);
    outb(ps2_cmdRegister, 0xFF);
    // Flush output buffer of PS/2 controller
    while (inb(ps2_statusRegister) & 0x01)
        inb(ps2_dataPort);
    int_registerHandler(IRQ_1, &kbd_callback);
    return 0;
}

void kbd_callback(registers_t* args) {
    inb(ps2_dataPort);
}
