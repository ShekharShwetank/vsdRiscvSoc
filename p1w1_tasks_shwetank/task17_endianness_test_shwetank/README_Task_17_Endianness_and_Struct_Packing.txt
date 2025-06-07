##################
# Task 17: Endianness & Struct Packing
# Author: Shwetank
# Date: June 7, 2025
# Program: India RISC-V Chip Tapeout
# Phase 1, Week 1
##################

### Objective:

The objective of this task was to answer the question "Is RV32 little-endian by default?" and to demonstrate a method to verify the system's byte ordering. This was accomplished by writing a C program that uses a `union` to inspect the byte-level memory representation of a multi-byte integer.

### Setup:

- **Toolchain:** xPack GNU RISC-V Embedded GCC v14.2.0-3
- **Emulator:** QEMU v8.2.2 (`qemu-system-riscv32`)
- **Files Used:**
    - `endian_test.c`: The C application containing the endianness test logic.
    - `interrupt_linker.ld`, `crt0.S`, `syscalls.c`: Reused from previous tasks to provide a working C environment with `printf` support.

### Core Concepts Explained:

The program relied on two key concepts:

1.  **Endianness:** This defines the order of bytes for a multi-byte number in computer memory. Our test was designed to distinguish between:
    - **Little-Endian:** Stores the least significant byte (LSB) at the lowest memory address.
    - **Big-Endian:** Stores the most significant byte (MSB) at the lowest memory address.

2.  **The C `union` Trick:** A `union` is a C data structure that allows its members to share the same memory space. We created a `union` with two members: a `uint32_t` and an array of four `uint8_t`s. This allowed us to write a 4-byte value using the integer member and then read back the individual bytes using the array member, directly revealing how they were ordered in memory.

### Implementation and Verification:

1.  **`endian_test.c`:** The C program defined a `union` as described above. It assigned the integer member the value `0x01020304`. It then used our retargeted `printf` to print the values of `checker.bytes[0]`, `checker.bytes[1]`, `checker.bytes[2]`, and `checker.bytes[3]`.

2.  **Compilation:** The program was compiled using the command:
    `riscv-none-elf-gcc -march=rv32imac_zicsr ... -o endian_test.elf crt0.S endian_test.c syscalls.c`

3.  **Execution & Observed Outcome:** The program was run in QEMU, producing the following output:
    ```
    --- RISC-V Endianness Test ---
    Value as a 32-bit integer: 0x01020304

    The same value stored in memory, byte by byte:
    Memory Address |  Value
    ----------------|--------
    Base Address + 0 |  0x04
    Base Address + 1 |  0x03
    Base Address + 2 |  0x02
    Base Address + 3 |  0x01

    Result: The system is Little-Endian.
    (The least significant byte, 0x04, is at the lowest address.)
    ```

### Conclusion:

The output empirically proves that the `bytes[0]` member of the union (at the lowest memory address) contained `0x04`, the least significant byte of the integer `0x01020304`. This confirms that our RISC-V toolchain and QEMU emulator are configured for a **Little-Endian** architecture, which is the standard defined by the RISC-V ISA specification.

### Status for Task 17: COMPLETED.
### ALL TASKS COMPLETED
