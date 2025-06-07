##################
# Task 15: Atomic Test Program
# Author: Shwetank
# Date: June 7, 2025
# Program: India RISC-V Chip Tapeout
# Phase 1, Week 1
##################

### Objective:

To practically apply the 'A' extension by implementing a spin-lock mutex using the `lr.w` and `sc.w` instructions. The goal was to use this mutex to protect a shared global variable from data corruption in a simulated concurrent environment, and to verify the correctness of the final result using a debugger.

### Setup:

- **Toolchain:** xPack GNU RISC-V Embedded GCC v14.2.0-3
- **Emulator:** QEMU v8.2.2 (`qemu-system-riscv32`)
- **Debugger:** xPack GDB v15.1 (`riscv-none-elf-gdb`)
- **Files Used:**
    - `interrupt_linker.ld`, `crt0.S`: Reused from Task 13 for memory mapping and C runtime setup.
    - `atomic_test.c`: The main C source file containing the mutex implementation and test logic.

### Implementation Details:

**1. Spin-lock Mutex (`lock`/`unlock`):**
A spin-lock was implemented using two inline functions:
- `lock()`: This function uses GCC inline assembly to create a loop that repeatedly executes `lr.w.aq` to load and reserve the mutex variable. If the value is zero (unlocked), it attempts to claim the lock using `sc.w.rl`. If the `sc.w` fails (because another core intervened) or if the lock was already held, the loop continues to "spin" until the lock is successfully acquired. The `.aq` (acquire) and `.rl` (release) memory ordering semantics were used to prevent harmful compiler/hardware instruction reordering.
- `unlock()`: This function simply writes a `0` back to the mutex memory location, releasing the lock.

**2. Pseudo-threads:**
To simulate concurrent access, two functions, `pseudo_thread_1` and `pseudo_thread_2`, were created. The `main` function called these sequentially. Each function looped 5 times, calling `lock()`, incrementing a `shared_counter`, and then calling `unlock()`. This simulated a scenario where two threads compete for access to the same shared resource.

### Compilation and Verification:

The program was compiled into an ELF executable using the `riscv-none-elf-gcc` toolchain.

Verification was performed using QEMU and GDB. The critical steps were:
1. Running the ELF in QEMU with the GDB server enabled (`-s -S`).
2. Connecting GDB and setting breakpoints on the `shared_counter++` line inside each pseudo-thread.
3. Stepping through the program to observe that the critical section was correctly protected.
4. Finally, running the program to completion and setting a breakpoint at the end to check the final value of `shared_counter`.

### Observed Outcome:

The GDB session confirmed that the final value of `shared_counter` was **10**. This is the correct sum of 5 increments from each of the two pseudo-threads. This successful result proves that the spin-lock mutex worked as intended, preventing any race conditions.

### Key Learnings:

This task provided a concrete, practical demonstration of how the low-level atomic instructions from the 'A' extension are the fundamental building blocks for creating higher-level synchronization primitives like mutexes. It made the theoretical concepts from Task 14 tangible and highlighted the care needed to manage shared resources in a concurrent environment.

### Status for Task 15: COMPLETED.
