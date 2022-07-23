; Declare constants for the multiboot header.
MBALIGN  equ  1 << 0            ; align loaded modules on page boundaries
MEMINFO  equ  1 << 1            ; provide memory map
FLAGS    equ  MBALIGN | MEMINFO ; this is the Multiboot 'flag' field
MAGIC    equ  0x1BADB002        ; 'magic number' lets bootloader find the header
CHECKSUM equ -(MAGIC + FLAGS)   ; checksum of above, to prove we are multiboot
 
; Declare a multiboot header that marks the program as a kernel. These are magic
; values that are documented in the multiboot standard. The bootloader will
; search for this signature in the first 8 KiB of the kernel file, aligned at a
; 32-bit boundary. The signature is in its own section so the header can be
; forced to be within the first 8 KiB of the kernel file.
section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

; The multiboot standard does not define the value of the stack pointer register
; (esp) and it is up to the kernel to provide a stack. This allocates room for a
; small stack by creating a symbol at the bottom of it, then allocating 16384
; bytes for it, and finally creating a symbol at the top. The stack grows
; downwards on x86. The stack is in its own section so it can be marked nobits,
; which means the kernel file is smaller because it does not contain an
; uninitialized stack. The stack on x86 must be 16-byte aligned according to the
; System V ABI standard and de-facto extensions. The compiler will assume the
; stack is properly aligned and failure to align the stack will result in
; undefined behavior.
section .bss
align 16
stack_bottom:
resb 16384 ; 16 KiB
stack_top:

section .text
global _start:function (_start.end - _start)
_start:
    ; Once into 32-bit protected mode through GRUB, Interrupts and Paging are 
    ; disabled. The Zeanix kernel has full access to the CPU, but no access to
    ; standard libraries without providing them. To begin, Zeanix moves into
    ; ESP the pointer to the top of the stack.

    mov esp, stack_top

    ; Move the multiboot header pointer into the value pointed to by 
    ; multiboot_ptr from ebx (used for finding multiboot modules, such as initrd.tar)

    mov [multiboot_ptr], ebx

    ; The high level kernel is then called, which will manage various parts of 
    ; the OS.

    extern kmain
    call kmain

    ; If the kernel ever exits, there will eventually be a kExitHandle, but for
    ; now the computer will just lock

    cli
.hang: hlt
    jmp .hang
.end:

global gdt_flush
gdt_flush:
    mov eax, [esp + 4]  ; Get GDT pointer (put on stack as an arg)
    lgdt [eax]          ; Call LGDT to load the new GDT

    mov ax, 0x10    ; 0x10 is the data offset, load all segment registers
    mov ds, ax      ; with it
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.flush ; 0x08 is the offset to our code segment, far jump must 
                    ; be performed
.flush:
    ret

global tss_flush
tss_flush:
    mov ax, 0x28
    ltr ax

section .data
global multiboot_ptr
multiboot_ptr:
    dd 0
