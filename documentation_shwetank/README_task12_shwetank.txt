################################################################################
# Task 12: Start-up Code & crt0
# Author: Shwetank
# Date: June 6, 2025
# Program: India RISC-V Chip Tapeout
# Phase 1, Week 1
################################################################################

Objective:
This task is informational, aimed at understanding the purpose and typical
responsibilities of the C Run-Time startup code (`crt0`) in a bare-metal
RISC-V program. It also covers where to find such startup code.

---
## I. What is `crt0` and Why is it Necessary?

In a C/C++ program, the `main()` function is the logical start for the user's
code, but it is not the true entry point of the executable. Before `main()` can
be called, the C runtime environment must be properly initialized. On a desktop
operating system, the OS loader and system C library handle this setup.

In a **bare-metal** environment (with no OS), this setup must be performed by a
special piece of startup code. This code is often called `crt0` (C Run-Time startup),
and the object file is `crt0.o`. It is typically written in assembly (`crt0.S`)
because it needs to perform low-level CPU initializations before the stack is even
available.

When we compiled with the `-nostartfiles` flag in Task 11, we were explicitly telling
the linker *not* to include the toolchain's default `crt0.o`, which is why we had
to provide our own `_start` label as the entry point. A complete `crt0.S` is
essentially a more robust and feature-complete version of the `_start` routine.

---
## II. Typical Responsibilities of `crt0.S`

The `crt0` code's primary job is to bridge the gap between the raw, post-reset
state of the processor and the state required to execute a standard C `main()`
function.

**1. Initialize the Stack Pointer (`sp`, `x2`):**
   - **Why:** The stack is essential for function calls (storing return addresses),
     passing arguments, and allocating local variables. Without a valid stack
     pointer, no C function can be called.
   - **How:** The startup code loads a known valid memory address into the `sp`
     register. This address typically points to the *top* (highest address) of a
     memory region reserved for the stack, which is defined in the linker script
     (e.g., via a symbol like `_stack_top`).

**2. Clear the `.bss` Section:**
   - **Why:** The C standard mandates that all global and static variables that are not
     explicitly initialized must be treated as if they were initialized to zero. These
     variables reside in a memory section called `.bss`. The executable file does not
     contain data for `.bss`; it only reserves space for it.
   - **How:** The startup code iterates through the `.bss` section in RAM, writing zeros
     to every byte. It finds the start and end of this section using symbols provided
     by the linker script (e.g., `__bss_start__` and `__bss_end__`).

**3. Initialize the `.data` Section (Copy from Flash to RAM):**
   - **Why:** The `.data` section contains global and static variables that have
     initial non-zero values (e.g., `int my_var = 10;`). These initial values must
     be loaded into writable RAM before `main()` begins. However, the values themselves
     must be stored in non-volatile memory (like Flash) to persist when the device is
     powered off.
   - **How:** The startup code performs a copy operation. It reads the initial data
     from its **Load Memory Address (LMA)** in Flash and writes it to its
     **Virtual Memory Address (VMA)** in RAM. The linker script provides symbols
     (e.g., `_sidata`, `_sdata`, `_edata`) to guide this copy loop. This concept
     builds directly on the LMA vs. VMA discussion from Task 11.

**4. Call `main()`:**
   - **Why:** This is the entry point for the user's application logic.
   - **How:** After the environment is fully initialized, `crt0` makes a standard
     function call to `main`. In a simple bare-metal system, `argc` and `argv`
     (arguments to `main`) are often hardcoded to 0 and NULL.

**5. Handle `main()`'s Return (The "Exit" Strategy):**
   - **Why:** A standard C `main` function returns an integer. In an OS, this signals an
     exit status. In a bare-metal system, there's no OS to return to, so the processor
     must be given something to do to prevent it from fetching and executing invalid
     code from whatever memory lies after the program.
   - **How:**
     - **Infinite Loop:** The most common strategy for deeply embedded systems is to
       enter an infinite loop (e.g., `exit_loop: j exit_loop`). This effectively
       halts the processor in a safe, predictable state.
     - **Call `exit()`:** `crt0` might call a library function `exit()`, which may
       perform cleanup (like calling global C++ destructors) before ultimately
       entering an infinite loop or triggering a system reset/halt via a debug interface.

---
## III. Where to Get `crt0.S`

As highlighted in `p1w1.pdf`, `crt0` files are not one-size-fits-all. They are often
specific to the C library, processor, and hardware memory map.

1.  **Toolchain-Provided (`newlib`):**
    - Our xPack toolchain, like most bare-metal toolchains, comes bundled with a `newlib`
      C library. This library includes a default `crt0.o` object file. When we compile
      without `-nostartfiles`, the linker automatically includes this default startup code.
    - The source (`crt0.S`) can be found within the `newlib` source code distribution. It's
      a great reference but is often generic.

2.  **Device-Specific Examples (SDKs/HALs):**
    - When working with a specific commercial microcontroller (e.g., from SiFive,
      STMicroelectronics, NXP), the vendor's Software Development Kit (SDK) or Hardware
      Abstraction Layer (HAL) will provide a tailored `crt0.S`. This is usually the
      best choice as it's written specifically for that chip's memory map (Flash/RAM
      addresses) and often includes essential hardware initializations (like setting
      up the clock system) before `main` is called.

3.  **Write Your Own:**
    - For full control, learning, or highly customized systems, developers write their
      own `crt_0.S`. Our `_start` function in Task 11 was a tiny, incomplete version
      of this.

---
## IV. Conclusion for Task 12

This informational task clarifies the critical "behind-the-scenes" work required to
run a C program on a bare-metal system. It connects the concepts of linker scripts
(Task 11), which define the memory map and symbols (`_stack_top`, `__bss_start__`,
etc.), with the startup code (`crt0`) that uses these symbols to prepare the
runtime environment for `main`.

Status for Task 12: COMPLETED.
