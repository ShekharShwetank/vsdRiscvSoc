/*
 * File: printf_test.c
 * Author: Shwetank
 * Date: June 7, 2025
 * Task: p1w1 - Task 16: Retargeting printf
 * Purpose: A simple program to test our retargeted _write syscall.
 */

#include <stdio.h>

int main() {
    int count = 0;

    printf("Hello, RISC-V world! Shwetank is here, testing printf.\n");
    printf("This is a test of the retargeted _write function.\n");

    while(1) {
        printf("The count is: %d\n", count++);
        // A simple delay loop
        for (volatile int i = 0; i < 1000000; ++i);
    }

    return 0;
}
