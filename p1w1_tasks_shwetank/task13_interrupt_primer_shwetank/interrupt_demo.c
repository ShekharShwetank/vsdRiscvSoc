/*
 * File: interrupt_demo.c
 * Author: Shwetank
 * Date: June 6, 2025
 * Task: p1w1 - Task 13: Interrupt Primer
 * Purpose: Demonstrates enabling and handling a machine-timer interrupt.
 */

#include <stdint.h>
#include <stdio.h>

// For this bare-metal example, we define the memory-mapped address
// for the machine time compare register (mtimecmp) for hart 0.
// This is typically part of a platform-specific header.
#define MTIME_CMP_ADDR 0x02004000
#define MTIME_ADDR     0x0200BFF8

// A global variable to be modified by the interrupt handler
volatile uint32_t interrupt_count = 0;

// The Interrupt Service Routine (ISR)
// The 'interrupt("machine")' attribute tells GCC to generate
// correct prologue/epilogue code for a machine-level interrupt handler,
// including saving all registers and using 'mret' to return.
void __attribute__((interrupt("machine"))) machine_timer_isr(void) {
    // 1. Acknowledge the interrupt and schedule the next one.
    // We do this by writing a new value to mtimecmp.
    volatile uint64_t *mtimecmp = (uint64_t *)MTIME_CMP_ADDR;
    volatile uint64_t *mtime = (uint64_t *)MTIME_ADDR;

    // Schedule the next interrupt for ~1 second from now (assuming a 1MHz clock)
    uint64_t next_interrupt_time = *mtime + 1000000;
    *mtimecmp = next_interrupt_time;

    // 2. Do the "work" of the ISR
    interrupt_count++;
}

// Function to set up the interrupt handler
void setup_interrupts(void) {
    // 1. Set mtvec to point to our ISR
    uintptr_t isr_address = (uintptr_t)machine_timer_isr;
    asm volatile("csrw mtvec, %0" : : "r"(isr_address));

    // 2. Enable Machine Timer Interrupts (MTIE bit in mie)
    uint32_t mie_val;
    asm volatile("csrr %0, mie" : "=r"(mie_val));
    mie_val |= (1 << 7); // Set MTIE (bit 7)
    asm volatile("csrw mie, %0" : : "r"(mie_val));
}

int main() {
    // Set up the interrupt handler address and enable timer interrupts
    setup_interrupts();

    // Schedule the first interrupt
    volatile uint64_t *mtimecmp = (uint64_t *)MTIME_CMP_ADDR;
    volatile uint64_t *mtime = (uint64_t *)MTIME_ADDR;
    *mtimecmp = *mtime + 1000000; // ~1 second from now

    // 3. Enable Global Interrupts (MIE bit in mstatus)
    uint32_t mstatus_val;
    asm volatile("csrr %0, mstatus" : "=r"(mstatus_val));
    mstatus_val |= (1 << 3); // Set MIE (bit 3)
    asm volatile("csrw mstatus, %0" : : "r"(mstatus_val));

    // Infinite loop, waiting for interrupts
    // The 'wfi' instruction puts the core to sleep until an interrupt occurs.
    while (1) {
        asm volatile("wfi");
    }

    return 0; // Should never be reached
}
