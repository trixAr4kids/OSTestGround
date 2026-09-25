#include <stdint.h>

// Assembly function prototype
extern void cpuid_native(uint32_t code, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx);

// Simple VGA Text Mode Driver
static volatile uint16_t* const VGA_BUFFER = (uint16_t*)0xB8000;
static int cursor_row = 0;
static int cursor_col = 0;

void clear_screen(void) {
    for (int i = 0; i < 80 * 25; i++) {
        VGA_BUFFER[i] = (uint16_t)0x0720;
    }
    cursor_row = 0;
    cursor_col = 0;
}

void print_char(char c) {
    if (c == '\n') {
        cursor_row++;
        cursor_col = 0;
        return;
    }
    int index = cursor_row * 80 + cursor_col;
    VGA_BUFFER[index] = (uint16_t)c | ((uint16_t)0x07 << 8); // Light grey on black
    cursor_col++;
    if (cursor_col >= 80) {
        cursor_col = 0;
        cursor_row++;
    }
}

void print_string(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        print_char(str[i]);
    }
}

// Read CPU Vendor ID (e.g., "GenuineIntel" or "AuthenticAMD")
void get_cpu_vendor(char *vendor) {
    uint32_t eax, ebx, ecx, edx;
    cpuid_native(0, &eax, &ebx, &ecx, &edx);

    // EBX, EDX, ECX contain the 12-byte string
    *(uint32_t*)&vendor[0] = ebx;
    *(uint32_t*)&vendor[4] = edx;
    *(uint32_t*)&vendor[8] = ecx;
    vendor[12] = '\0';
}

// Read CPU Brand String
void get_cpu_brand(char *brand) {
    uint32_t eax, ebx, ecx, edx;
    
    // Check if extended CPUID leaves are supported
    cpuid_native(0x80000000, &eax, &ebx, &ecx, &edx);
    if (eax < 0x80000004) {
        const char *unk = "Brand String Unsupported";
        for(int i = 0; i < 25; i++) brand[i] = unk[i];
        return;
    }

    // Leaf 0x80000002: Bytes 0-15
    cpuid_native(0x80000002, &eax, &ebx, &ecx, &edx);
    *(uint32_t*)&brand[0]  = eax;
    *(uint32_t*)&brand[4]  = ebx;
    *(uint32_t*)&brand[8]  = ecx;
    *(uint32_t*)&brand[12] = edx;

    // Leaf 0x80000003: Bytes 16-31
    cpuid_native(0x80000003, &eax, &ebx, &ecx, &edx);
    *(uint32_t*)&brand[16] = eax;
    *(uint32_t*)&brand[20] = ebx;
    *(uint32_t*)&brand[24] = ecx;
    *(uint32_t*)&brand[28] = edx;

    // Leaf 0x80000004: Bytes 32-47
    cpuid_native(0x80000004, &eax, &ebx, &ecx, &edx);
    *(uint32_t*)&brand[32] = eax;
    *(uint32_t*)&brand[36] = ebx;
    *(uint32_t*)&brand[40] = ecx;
    *(uint32_t*)&brand[44] = edx;

    brand[48] = '\0';
}

void kernel_main(void) {
    clear_screen();

    char vendor[13];
    char brand[49];

    get_cpu_vendor(vendor);
    get_cpu_brand(brand);

    print_string("CPUID Read Succesfully, Results:\n\n");
    print_string("Vendor: ");
    print_string(vendor);
    print_string("\n");

    print_string("Brand : ");
    print_string(brand);
    print_string("\n");
}