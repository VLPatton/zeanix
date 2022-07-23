%macro ISR_NOERRCODE 1
global isr%1
isr%1:
    push dword 0    ; Push fake error code
    push %1   ; Push Interrupt/Exception Number
    jmp isr_common_stub
%endmacro

%macro ISR_ERRCODE 1
global isr%1
isr%1:
    push %1
    jmp isr_common_stub
%endmacro 

section .text
align 4

global idt_flush
idt_flush:
    cli
    mov eax, [esp + 4]
    lidt [eax]
    sti
    ret

ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE   8
ISR_NOERRCODE 9
ISR_ERRCODE   10
ISR_ERRCODE   11
ISR_ERRCODE   12
ISR_ERRCODE   13
ISR_ERRCODE   14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_NOERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31

extern isr_handler

isr_common_stub:
    pusha                       ; Push most GP registers to save state
    mov ax, ds                  ; Save current DS so we can return to it if we were in usermode
    push eax
    mov ax, 0x10                ; Kernel Data Segment (third entry in GDT)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    push esp                    ; At this point ESP is a pointer to where GS (and the rest
                                ; of the interrupt handler state resides)
                                ; Push ESP as 1st parameter as it's a 
                                ; pointer to a registers_t 

    call isr_handler            ; ISR handler will call other handling functions depending on exception
                                ; or it may cause a Kernel Panic

    pop ebx                     ; Remove the saved ESP on the stack. Efficient to just pop it 
                                ; into any register. You could have done: add esp, 4 as well
    
    pop ebx                     ; Return to previous Data Segment
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx

    popa                        ; Pop previous state of registers
    add esp, 8                  ; Fix stack by removing pushed err. code and int. no.

    iret                        ; Return from interrupt and restore processor state

%macro IRQ 2
global irq%1
irq%1:
    cli
    push dword 0
    push dword %2
    jmp irq_common_stub
%endmacro

IRQ 0,  32
IRQ 1,  33
IRQ 2,  34
IRQ 3,  35
IRQ 4,  36
IRQ 5,  37
IRQ 6,  38
IRQ 7,  39
IRQ 8,  40
IRQ 9,  41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47
IRQ 0x69, 0x69

extern irq_handler

global irq_common_stub
irq_common_stub:
    pusha                       ; Push most GP registers to save state
    mov ax, ds                  ; Save current DS so we can return to it if we were in usermode
    push eax
    mov ax, 0x10                ; Kernel Data Segment (third entry in GDT)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    push esp                    ; At this point ESP is a pointer to where GS (and the rest
                                ; of the interrupt handler state resides)
                                ; Push ESP as 1st parameter as it's a 
                                ; pointer to a registers_t 

    call irq_handler            ; IRQ handler will call other parts of kernel or signal other
                                ; processes about a hardware interrupt

    pop ebx                     ; Remove the saved ESP on the stack. Efficient to just pop it 
                                ; into any register. You could have done: add esp, 4 as well
    
    pop ebx                     ; Return to previous Data Segment
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx

    popa                        ; Pop previous state of registers
    add esp, 8                  ; Fix stack by removing pushed err. code and int. no.

    iret                        ; Return from interrupt and restore processor state
