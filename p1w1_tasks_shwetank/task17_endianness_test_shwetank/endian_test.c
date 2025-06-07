/*
 * File: endian_test.c
 * Author: Shwetank
 * Date: June 7, 2025
 * Task: p1w1 - Task 17: Endianness Test
 * Purpose: To verify the endianness of the RISC-V system.
 */

#include <stdio.h>
#include <stdint.h>

// A union to inspect byte ordering
typedef union {
    uint32_t integer;
    uint8_t  bytes[4];
} endian_checker_t;

int main() {
    endian_checker_t checker;
    checker.integer = 0x01020304; // Assign a known 4-byte value

    printf("--- RISC-V Endianness Test ---\n");
    printf("Value as a 32-bit integer: 0x%08X\n", checker.integer);
    printf("\n");
    printf("The same value stored in memory, byte by byte:\n");
    printf("Memory Address |  Value\n");
    printf("----------------|--------\n");
    printf("Base Address + 0 |  0x%02X\n", checker.bytes[0]);
    printf("Base Address + 1 |  0x%02X\n", checker.bytes[1]);
    printf("Base Address + 2 |  0x%02X\n", checker.bytes[2]);
    printf("Base Address + 3 |  0x%02X\n", checker.bytes[3]);
    printf("\n");

    if (checker.bytes[0] == 0x04) {
        printf("Result: The system is Little-Endian.\n");
        printf("(The least significant byte, 0x04, is at the lowest address.)\n");
    } else {
        printf("Result: The system is Big-Endian.\n");
    }

    while(1); // Halt
    return 0;
}
