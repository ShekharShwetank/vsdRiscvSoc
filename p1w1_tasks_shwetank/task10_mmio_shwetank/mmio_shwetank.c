// File: mmio_shwetank.c
// Author: Shwetank
// Date: June 5, 2025
// Task: p1w1.pdf - Task 10: Memory-Mapped I/O Demo
// Purpose: Demonstrates accessing a hypothetical memory-mapped GPIO register
//          and the importance of the 'volatile' keyword.

#include <stdio.h>
#include <stdint.h> // Required for uint32_t

// Define the hypothetical address of our GPIO data register
// This address is arbitrary for this exercise. In a real system,
// it would be defined in the hardware documentation.
#define GPIO_DATA_REGISTER_ADDRESS 0x10012000

int main() {
    // Create a volatile pointer to the GPIO register.
    // The address 0x10012000 is cast to a pointer to a 32-bit unsigned integer.
    volatile uint32_t *gpio_data_reg = (uint32_t *)GPIO_DATA_REGISTER_ADDRESS;

    printf("MMIO Demo by Shwetank\n");
    printf("Targeting hypothetical GPIO register at address: 0x%08X\n", (uint32_t)GPIO_DATA_REGISTER_ADDRESS);

    // Action 1: Write 0x1 to the GPIO register (e.g., turn something ON)
    // The 'volatile' keyword ensures this write operation is actually performed
    // and not optimized away by the compiler, even if the compiler doesn't see
    // any subsequent reads from this variable in this specific code block.
    printf("Writing 0x00000001 to GPIO register...\n");
    *gpio_data_reg = 0x00000001;
    // In a real system, this would change the state of GPIO pins.
    // In our simulation (Spike+pk without this address being a real device),
    // this write will go to memory if writable, or might cause an exception
    // if the memory address is invalid or not writable by user code.
    // For this exercise, we assume it's a valid operation for demonstration.

    // Perform some dummy work
    for (volatile int i = 0; i < 5000; i++);

    // Action 2: Write 0x0 to the GPIO register (e.g., turn something OFF)
    printf("Writing 0x00000000 to GPIO register...\n");
    *gpio_data_reg = 0x00000000;

    printf("Finished writing to GPIO register.\n");

    // Note: To actually observe the effect of these writes, this program would
    // need to be run on hardware or an emulator that models a device at
    // 0x10012000. For this task, understanding the C construct and 'volatile'
    // is the primary goal. We use printf (via pk) for demonstration messages.

    return 0;
}
