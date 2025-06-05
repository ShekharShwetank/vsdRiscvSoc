// File: bare_metal_app_shwetank.c
// Author: Shwetank
// Purpose: Minimal bare-metal C app to test the linker script.

#include <stdint.h>

// Initialized global variable (should go into .data section)
uint32_t initialized_global_data = 0x12345678;

// Uninitialized global variable (should go into .bss section)
uint32_t uninitialized_global_data;

// Constant global variable (should go into .rodata section, part of .text segment)
const uint32_t constant_global_data = 0xAABBCCDD;

// Entry point for bare-metal execution
void _start() {
    // In a real application, you would:
    // 1. Initialize the stack pointer (sp).
    // 2. Copy .data section from Flash (LMA) to RAM (VMA).
    // 3. Zero out the .bss section in RAM.
    // 4. Call main().

    // For this test, just a simple operation and an infinite loop.
    initialized_global_data = 0xFFFFFFFF; 
    uninitialized_global_data = 0xEEEEEEEE;

    volatile int loop = 1;
    while(loop) {
        // Loop forever
    }
}
