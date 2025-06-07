/*
 * File: atomic_test.c
 * Author: Shwetank
 * Date: June 7, 2025
 * Task: p1w1 - Task 15: Atomic Test Program
 * Purpose: Implements a spin-lock mutex using lr.w/sc.w
 * and tests it with two pseudo-threads.
 */

#include <stdint.h>

// Our mutex will be a simple unsigned integer.
// 0: unlocked, 1: locked
typedef volatile uint32_t mutex_t;

// A shared variable to be protected by the mutex
volatile uint32_t shared_counter = 0;

// A variable to signal when the program is done
volatile int test_complete = 0;

// --- Spin-lock Implementation ---

static inline void lock(mutex_t *mutex) {
    uint32_t lock_val = 1; // The value we want to store to acquire the lock
    uint32_t temp_reg;

    asm volatile (
        "1: "
        "lr.w.aq %[temp], (%[mutex_addr])\n\t" // Load-reserved and acquire
        "bnez %[temp], 1b\n\t"               // If lock is not 0, it's held, so spin
        "sc.w.rl %[temp], %[lock_val], (%[mutex_addr])\n\t" // Attempt to store-conditional and release
        "bnez %[temp], 1b\n\t"               // If store failed, loop and retry
        : [temp] "=&r" (temp_reg)
        : [mutex_addr] "r" (mutex), [lock_val] "r" (lock_val)
        : "memory"
    );
}

static inline void unlock(mutex_t *mutex) {
    // A simple store of 0 is sufficient to unlock.
    // The 'amoswap' instruction could also be used here for a guaranteed atomic write.
    asm volatile (
        "sw zero, (%[mutex_addr])"
        :
        : [mutex_addr] "r" (mutex)
        : "memory"
    );
}

// --- Pseudo-thread Implementation ---

void pseudo_thread_1(mutex_t *lock_addr) {
    for (int i = 0; i < 5; ++i) {
        lock(lock_addr);
        // --- Critical Section Start ---
        shared_counter++;
        // --- Critical Section End ---
        unlock(lock_addr);
    }
}

void pseudo_thread_2(mutex_t *lock_addr) {
    for (int i = 0; i < 5; ++i) {
        lock(lock_addr);
        // --- Critical Section Start ---
        shared_counter++;
        // --- Critical Section End ---
        unlock(lock_addr);
    }
}


int main() {
    // The mutex, initialized to 0 (unlocked)
    mutex_t my_mutex = 0;

    // Run our two "threads"
    pseudo_thread_1(&my_mutex);
    pseudo_thread_2(&my_mutex);

    // After both threads have run, the final value of the shared counter
    // should be 10 (5 increments from each thread).
    // A real test would have a check here, but we will verify with GDB.
    
    test_complete = 1; // Set a flag for GDB to check

    while(1) {
        // Loop forever
    }

    return 0;
}
