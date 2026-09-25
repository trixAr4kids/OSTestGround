bits 32

section .multiboot
    align 4
    dd 0x1BADB002              ; Magic number for Multiboot v1
    dd 0x00                     ; Flags
    dd -(0x1BADB002 + 0x00)     ; Checksum (magic + flags + checksum = 0)

section .text
global _start
extern kernel_main

_start:
    cli                         ; Clear interrupts
    mov esp, stack_top          ; Set up stack pointer
    call kernel_main            ; Call main function in C

.hang:
    hlt                         ; Halt CPU if kernel returns
    jmp .hang

section .bss
align 16
stack_bottom:
    resb 16384                  ; Reserve 16 KB for kernel stack
stack_top: