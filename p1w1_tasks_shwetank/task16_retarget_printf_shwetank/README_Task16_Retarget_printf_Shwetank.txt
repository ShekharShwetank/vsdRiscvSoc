##################
# Task 16: Using Newlib printf Without an OS
# Author: Shwetank
# Date: June 7, 2025
# Program: India RISC-V Chip Tapeout
# Phase 1, Week 1
##################

### Objective:

To make the standard C library function `printf` fully operational in our bare-metal RISC-V environment. This was achieved by implementing a "syscall shim" layer, most notably the `_write` function, to redirect character output to the QEMU emulated UART.

### Setup:

- **Toolchain:** xPack GNU RISC-V Embedded GCC v14.2.0-3
- **Emulator:** QEMU v8.2.2 (`qemu-system-riscv32`)
- **Files Created/Used:**
    - `printf_test.c`: The main C application that calls `printf`.
    - `syscalls.c`: A new file containing our implementations of required low-level C library syscalls.
    - `interrupt_linker.ld`, `crt0.S`: Reused from previous tasks.

### Implementation Details:

**1. The Concept - Retargeting Syscalls:**
The `newlib` C library, used by our toolchain, is designed for embedded systems. High-level functions like `printf` rely on a set of low-level "syscall" stubs to interact with hardware. To make them work in a bare-metal environment, we must provide our own implementations for these stubs.

**2. Syscall Shim (`syscalls.c`):**
A new C file was created to house our syscall implementations:
- **`_write()`:** This function was implemented to receive a buffer of characters and a length from `printf`. It then loops through the buffer, writing each character one-by-one to the memory-mapped QEMU UART transmit register at address `0x10000000`.
- **Other Stubs (`_sbrk`, `_close`, etc.):** The initial attempt failed because `printf` has other dependencies, notably `_sbrk` for memory (heap) allocation. A complete shim was implemented, including a simple "bump allocator" for `_sbrk` and minimal non-crashing stubs for `_close`, `_fstat`, `_isatty`, `_lseek`, and `_read`.

### Compilation and Verification:

**Compilation:**
The program was compiled with a modified command:
`riscv-none-elf-gcc -march=rv32imac_zicsr ... -ffreestanding -nostartfiles ... crt0.S printf_test.c syscalls.c`
- The key change was the **removal of the `-nostdlib` flag**. This was necessary to link `printf` and the rest of the C library from `newlib`.
- The `-nostartfiles` flag was kept, ensuring our own `crt0.S` was used as the entry point, not `newlib`'s default.

**Verification:**
The program was executed in QEMU:
`qemu-system-riscv32 -nographic -machine virt -bios none -kernel printf_test.elf`

The program successfully printed the formatted strings and the incrementing counter to the host terminal, confirming that `printf` was correctly retargeted to the UART.

### Key Learnings and Challenges:

The most significant challenge was the initial failure where the program consumed 100% CPU with no output.
- **Diagnosis:** This was traced to `printf` calling the `_sbrk` syscall, for which we had not provided an implementation. The default `newlib` stub for missing syscalls is often an infinite loop, which caused the CPU to spin uselessly.
- **Learning:** This task demonstrated that making a C library work in a bare-metal context requires more than just implementing the obvious functions like `_write`. A more complete "syscall shim" is needed to satisfy all library dependencies and prevent the program from trapping in default stubs.

### Status for Task 16: COMPLETED.
