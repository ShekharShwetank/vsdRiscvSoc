// File: inline_asm_shwetank.c
// Author: Shwetank
// Date: June 5, 2025
// Task: p1w1.pdf - Task 9: Inline Assembly Basics
// Purpose: Demonstrates reading the 'cycle' CSR using inline assembly.

#include <stdio.h>
#include <stdint.h> // Required for uint32_t

// Function to read the lower 32 bits of the time counter CSR (0xC01)
static inline uint32_t rdtime_csr(void) {
    uint32_t time_val;
    asm volatile (
        "csrr %0, time"  // Read CSR 'time' into register %0
        : "=r" (time_val) 
    );
    return time_val;
}

int main() {
    uint32_t time1, time2, diff;

    time1 = rdtime_csr();
    printf("Time count at start: %u\n", time1);

    for (volatile int i = 0; i < 10000; i++) {}

    time2 = rdtime_csr();
    printf("Time count at end: %u\n", time2);

    diff = time2 - time1; // This difference might be 0 if time isn't advancing rapidly in sim
    printf("Difference (time units): %u\n", diff);

    return 0;
}

