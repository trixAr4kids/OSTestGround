bits 32

global cpuid_native

section .text

; Function: void cpuid_native(uint32_t code, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx)
cpuid_native:
    push ebp
    mov ebp, esp
    push ebx
    push edi

    mov eax, [ebp + 8]    ; Leaf code
    cpuid

    mov edi, [ebp + 12]   ; &eax
    mov [edi], eax
    mov edi, [ebp + 16]   ; &ebx
    mov [edi], ebx
    mov edi, [ebp + 20]   ; &ecx
    mov [edi], ecx
    mov edi, [ebp + 24]   ; &edx
    mov [edi], edx

    pop edi
    pop ebx
    pop ebp
    ret