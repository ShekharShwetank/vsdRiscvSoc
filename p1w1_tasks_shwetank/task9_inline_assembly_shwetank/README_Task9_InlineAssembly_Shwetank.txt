################################################################################
# Task 9: Inline Assembly Basics
# Author: Shwetank
# Date: June 5, 2025
# Program: India RISC-V Chip Tapeout
# Phase 1, Week 1
################################################################################

Objective:
To write a C function using GCC inline assembly to read the `cycle` CSR (Control
and Status Register, address 0xC00), explain the constraints used, compile the
code, and attempt to run it. Also, to test reading the `time` CSR (0xC01)
to understand CSR accessibility.

Setup:
- Toolchain: xPack GNU RISC-V Embedded GCC v14.2.0-3 (riscv-none-elf-gcc).
- Emulator Environment: Spike v1.1.1-dev with Proxy Kernel (pk) built for
  `rv32imac_zicsr_zifencei`.
- Workspace: ~/riscv_tapeout_shwetank_workspace/p1w1_tasks_shwetank/task9_inline_assembly_shwetank/

C Source File (`inline_asm_shwetank.c` - final version tested with `time` CSR):
  // File: inline_asm_shwetank.c
  // Author: Shwetank
  // Purpose: Demonstrates reading 'time' CSR using inline assembly.
  // (Initially tried 'cycle' CSR as per p1w1.pdf, but also resulted in illegal instruction)

  #include <stdio.h>
  #include <stdint.h> // Required for uint32_t

  // Function to read the lower 32 bits of the time counter CSR (0xC01)
  static inline uint32_t rdtime_csr(void) {
      uint32_t time_val;
      asm volatile (
          "csrr %0, time"  // Assembly instruction: Read CSR 'time' into register %0
          : "=r" (time_val) // Output constraint: C variable 'time_val' is output, placed in any general-purpose register ('r')
      );
      return time_val;
  }

  int main() {
      uint32_t time1, time2, diff;

      time1 = rdtime_csr();
      printf("Time count at start: %u\n", time1); // This printf may not show if rdtime_csr traps

      for (volatile int i = 0; i < 10000; i++) {} // Dummy work

      time2 = rdtime_csr();
      printf("Time count at end: %u\n", time2);

      diff = time2 - time1;
      printf("Difference (time units): %u\n", diff);
      
      return 0;
  }

Compilation and Execution:

1. Compilation:
   - The C file uses inline assembly with `csrr`. Based on previous findings (Task 1),
     the `-march=rv32imac_zicsr` flag is necessary for the xPack assembler to
     recognize CSR instructions, even within inline assembly in C files.
   - Command Used (successful compilation):
     `riscv-none-elf-gcc -march=rv32imac_zicsr -mabi=ilp32 -g -o inline_asm_shwetank.elf inline_asm_shwetank.c`
     (Note: Compiled *without* --specs=nosys.specs or --specs=nano.specs to allow printf
      to attempt syscalls for pk).

2. Execution with Spike and `pk`:
   - Command:
     `spike --isa=rv32imac /home/ank/riscv_custom_tools/riscv-none-elf/bin/pk ./inline_asm_shwetank.elf`
   - Outcome (for both `csrr %0, cycle` and `csrr %0, time` attempts):
     A register dump was printed, followed by:
     `pc <address_of_csrr> va/inst <opcode_for_csrr> sr <sstatus_value>`
     `An illegal instruction was executed!`

Analysis of "Illegal Instruction" Error:
- The error occurs precisely when the `csrr` instruction (for both `cycle` and `time` CSRs) is executed.
- The xPack assembler correctly compiled the instruction (due to `-march=rv32imac_zicsr`).
- The runtime error indicates that the execution environment (Spike + `pk`) does not permit User-mode (U-mode) access to these counter CSRs.
- Access to `cycle` (0xC00), `time` (0xC01), and `instret` (0xC02) from U-mode is controlled by enable bits in the `mcounteren` (Machine Counter Enable) and `scounteren` (Supervisor Counter Enable) CSRs.
- The observed behavior strongly suggests that `pk`, in its current build and execution on Spike, is not setting the necessary bits in `mcounteren` and/or `scounteren` to allow the U-mode application to read these performance counters.

Explanation of Inline Assembly Syntax Used:
  `asm volatile ("csrr %0, csr_name" : "=r" (c_variable));`
  - `asm`: Keyword to denote an inline assembly block.
  - `volatile`: Instructs the compiler not to optimize the assembly block away or significantly reorder it. This is crucial for operations with side effects or direct hardware/register access like CSR reads, ensuring the instruction is executed as intended.
  - `"csrr %0, csr_name"`: The assembler instruction template.
    - `csrr`: The RISC-V instruction for "Control and Status Register Read".
    - `%0`: A placeholder for the first operand. It refers to the C variable associated with the first constraint.
    - `csr_name`: The symbolic name of the CSR to be read (e.g., `cycle`, `time`).
  - `:`: Separates the assembler template from output operands, then input operands, then clobbered registers.
  - `"=r" (c_variable)`: Defines an output operand.
    - `=`: Indicates it's an output operand (data flows from assembly to C).
    - `r`: A constraint specifying that the C variable should be stored in a general-purpose register for this operation. The compiler chooses a suitable register.
    - `(c_variable)`: The C lvalue (e.g., a variable name) that will receive the value read from the CSR via the chosen register.
  - The subsequent colons (for input operands and clobbered registers) were empty in this example, as the `csrr` instruction takes its CSR source implicitly and only has one explicit register destination.

Status for Task 9: COMPLETED(with minor issues)
- C function with inline assembly to read CSRs was correctly written and compiled.
- Explanation of constraints and `volatile` understood. 
- Execution in the Spike+`pk` environment FAILED due to an "Illegal instruction" error when attempting to access `cycle` or `time` CSRs. This is attributed to the execution environment not enabling User-mode access to these counters.
- The core objective of writing and understanding the inline assembly syntax is considered met, with the runtime issue being an important environmental finding.

Key Learnings:
- Even if assembly code (including inline assembly) compiles correctly, its runtime behavior depends on the execution environment's permissions and configuration (e.g., CSR accessibility based on privilege modes and enable bits like `mcounteren`/`scounteren`).
- `-march=rv32imac_zicsr` was necessary for the xPack toolchain to compile C code containing `csrr` inline assembly.
