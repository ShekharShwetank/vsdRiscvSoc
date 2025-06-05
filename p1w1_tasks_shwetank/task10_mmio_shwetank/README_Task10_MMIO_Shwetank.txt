################################################################################
# Task 10: Memory-Mapped I/O (MMIO) Demo
# Author: Shwetank
# Date: June 5, 2025
# Program: India RISC-V Chip Tapeout
# Phase 1, Week 1
################################################################################

Objective:
To understand and demonstrate how to access memory-mapped hardware registers
from C code, specifically focusing on:
1.  Writing a C snippet to interact with a hypothetical GPIO register at a given
    memory address (`0x10012000`).
2.  Understanding and using the `volatile` keyword to prevent the compiler from
    optimizing away essential hardware accesses.
3.  Observing the outcome when such code is run in a simulated environment like
    Spike with the Proxy Kernel (`pk`).

Setup:
- Toolchain: xPack GNU RISC-V Embedded GCC v14.2.0-3 (`riscv-none-elf-gcc`).
- Emulator Environment: Spike v1.1.1-dev with Proxy Kernel (`pk`) built for
  `rv32imac_zicsr_zifencei`.
- Workspace: ~/riscv_tapeout_shwetank_workspace/p1w1_tasks_shwetank/task10_mmio_shwetank/

Core Concept: Memory-Mapped I/O (MMIO)
---------------------------------------
Memory-Mapped I/O is a common technique in embedded systems and SoCs where
hardware peripheral control and data registers are not accessed via special
I/O CPU instructions (like `in`/`out` on x86). Instead, these hardware registers
are mapped to specific addresses within the processor's regular memory address
space.

To communicate with a peripheral (e.g., read its status, write a command, send
data), the CPU simply performs standard memory load and store operations to these
predefined addresses. For example, writing a particular value to a specific
address might configure a GPIO pin, while reading from another address might
get the current status of a UART.

C Source File (`mmio_shwetank.c`):
-----------------------------------
  // File: mmio_shwetank.c
  // Author: Shwetank
  // Date: June 5, 2025
  // Task: p1w1.pdf - Task 10: Memory-Mapped I/O Demo
  // Purpose: Demonstrates accessing a hypothetical memory-mapped GPIO register
  //          and the importance of the 'volatile' keyword.

  #include <stdio.h>
  #include <stdint.h> // Required for uint32_t

  // Define the hypothetical address of our GPIO data register
  #define GPIO_DATA_REGISTER_ADDRESS 0x10012000

  int main() {
      // Create a volatile pointer to the GPIO register.
      // The address 0x10012000 is cast to a pointer to an unsigned 32-bit integer.
      volatile uint32_t *gpio_data_reg = (uint32_t *)GPIO_DATA_REGISTER_ADDRESS;

      printf("MMIO Demo by Shwetank\n");
      printf("Targeting hypothetical GPIO register at address: 0x%08X\n", (uint32_t)GPIO_DATA_REGISTER_ADDRESS);

      // Action 1: Write 0x1 to the GPIO register (e.g., turn something ON)
      printf("Writing 0x00000001 to GPIO register...\n");
      *gpio_data_reg = 0x00000001; 
      // This line attempts to write to the memory address 0x10012000.

      // Perform some dummy work (to ensure some separation between writes if they were successful)
      for (volatile int i = 0; i < 5000; i++);

      // Action 2: Write 0x0 to the GPIO register (e.g., turn something OFF)
      // This line might not be reached if the first write causes a fault.
      printf("Writing 0x00000000 to GPIO register...\n");
      *gpio_data_reg = 0x00000000;

      printf("Finished writing to GPIO register (or attempted to).\n");

      return 0;
  }

Explanation of Key C Constructs:

1.  Pointer Casting: `(uint32_t *)GPIO_DATA_REGISTER_ADDRESS`
    - `GPIO_DATA_REGISTER_ADDRESS` (0x10012000) is an integer representing a memory address.
    - `(uint32_t *)` casts this integer into a "pointer to an unsigned 32-bit integer".
    - This tells the compiler to treat the memory at this address as if it holds a `uint32_t` value. When we dereference this pointer (`*gpio_data_reg`), the compiler will generate instructions to access 4 bytes (32 bits) at that address.

2.  The `volatile` Keyword:
    - `volatile uint32_t *gpio_data_reg;`
    - **Purpose:** The `volatile` qualifier is essential for MMIO. It informs the compiler that the value at the memory location pointed to by `gpio_data_reg` can change at any time due to means not controlled or observable by the compiler itself (e.g., hardware events, another thread, an interrupt service routine).
    - **Effects:**
        - **Prevents Optimization of Accesses:** Without `volatile`, if the compiler performs a write to `*gpio_data_reg` but doesn't see that value being subsequently read or used by the program, it might "optimize away" the write, assuming it's redundant. For MMIO, every write to a hardware register is significant and must occur. `volatile` ensures that every read from and every write to the pointer is actually performed as specified in the code.
        - **Prevents Reordering of Accesses:** Compilers might reorder memory read/write operations to improve performance. However, for hardware registers, the order of operations is often critical. `volatile` helps ensure that accesses to volatile variables are not reordered relative to other volatile accesses or specific sequence points, preserving the intended interaction sequence with the hardware.
    - In our code, `*gpio_data_reg = 0x00000001;` and `*gpio_data_reg = 0x00000000;` are writes to a hardware register. `volatile` ensures these stores are emitted by the compiler.

3.  Alignment:
    - The GPIO register is treated as a `uint32_t` (4 bytes). The address `0x10012000` is a multiple of 4, so it is naturally aligned for a 32-bit access.
    - While RISC-V can often handle misaligned accesses (potentially with a performance penalty or via trap-and-emulate), hardware peripheral registers usually require accesses to be aligned to their size. Using a correctly typed pointer to an aligned address is good practice for MMIO.

Compilation:
- Command Used:
  `riscv-none-elf-gcc -march=rv32imac -mabi=ilp32 -g -o mmio_shwetank.elf mmio_shwetank.c`
  (Compiled without --specs=nosys.specs to allow printf via pk).
- Outcome: Successful compilation.

Execution with Spike and `pk`:
- Command:
  `spike --isa=rv32imac /home/ank/riscv_custom_tools/riscv-none-elf/bin/pk ./mmio_shwetank.elf`
- Observed Output and Outcome:

MMIO Demo by Shwetank

Targeting hypothetical GPIO register at address: 0x10012000

Writing 0x00000001 to GPIO register...

z  00000000 ra 00010190 sp 7ffffd70 gp 0001e810

tp 00000000 t0 00010a24 t1 0001cae0 t2 00000001

s0 7ffffd90 s1 00000000 a0 0000000a a1 0001f008

a2 00000027 a3 00000027 a4 00000001 a5 10012000

a6 0000000a a7 00000040 s2 00000000 s3 00000000

s4 00000000 s5 00000000 s6 00000000 s7 00000000

s8 00000000 s9 00000000 sA 00000000 sB 00000000

t3 0001ca8c t4 00000000 t5 00008800 t6 00000080

pc 00010196 va/inst 10012000 sr 80006020

User store segfault @ 0x10012000 

Analysis of Segfault:
- The program executed the initial `printf` statements successfully via `pk`.
- When the line `*gpio_data_reg = 0x00000001;` was executed, a "User store segfault @ 0x10012000" occurred.
- This means the User-mode application attempted to write to memory address `0x10012000`, but this address was not mapped as writable for the user program by the Proxy Kernel (`pk`) in the Spike simulation environment.
- This is an expected outcome when accessing arbitrary memory locations that don't correspond to actual, writable memory-mapped peripherals modeled by the simulator or made available by the operating environment (`pk`).

Key Learnings:
- Correctly demonstrated the C syntax for MMIO using volatile pointers.
- Understood the critical role of `volatile` in preventing compiler optimizations that would break MMIO.
- Observed a segmentation fault when writing to a hypothetical MMIO address, illustrating that memory accesses are subject to permissions and mappings defined by the execution environment (hardware, OS, or supervisor like `pk`).
- To see an actual GPIO toggle, this code would need to be run on a system where `0x10012000` is a real, writable GPIO register, or in an emulator configured to model such a device at that address.

Status for Task 10: COMPLETED. The C snippet was correctly written, and the role of `volatile` was understood. The segfault provided a practical demonstration of memory protection in the simulated environment.
