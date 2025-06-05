################################################################################
# Task 2: Compile "Hello, RISC-V" (C Program)
# Author: Shwetank
# Date: June 5, 2025
# Program: India RISC-V Chip Tapeout
# Phase 1, Week 1
################################################################################

Objective:
To write a minimal "Hello, World!" C program, cross-compile it for the RV32IMAC target using the verified xPack toolchain, and confirm the properties of the resulting ELF executable. This task tests the toolchain's C compilation and linking capabilities with standard libraries.

Setup:
- Toolchain: xPack GNU RISC-V Embedded GCC v14.2.0-3 (riscv-none-elf-gcc).
- Workspace: ~/riscv_tapeout_shwetank_workspace/p1w1_tasks_shwetank/task2_hello_riscv_shwetank/

C Source File (`hello_riscv_shwetank.c`):
  // File: hello_riscv_shwetank.c
  // Author: Shwetank
  #include <stdio.h>

  int main() {
      printf("Hello, RISC-V! Shwetank is here.\n");
      return 0;
  }

Compilation Process with xPack Toolchain:

1. Initial Compilation (for GDB `target sim` testing, later found problematic for emulator output):
   - Command:
     `riscv-none-elf-gcc -march=rv32imac -mabi=ilp32 -g -o hello_riscv_xpack.elf hello_riscv_shwetank.c --specs=nosys.specs --specs=nano.specs`
   - Outcome: Successfully compiled and linked. Produced `hello_riscv_xpack.elf`.
   - Linker Warnings: Showed expected warnings about unimplemented syscalls (e.g., `_write`, `_close`) due to `nosys.specs`. This meant `printf` wouldn't produce console output without further work (like `pk` or custom syscalls).

2. Recompilation for `pk` (SUCCESSFUL for emulator output in Task 7):
   - Rationale: To enable `printf` output via the Proxy Kernel (`pk`), the program needs to make actual syscalls (`ecall`) that `pk` can intercept. This typically requires linking with a more complete C library without `nosys.specs`.
   - Command:
     `riscv-none-elf-gcc -march=rv32imac -mabi=ilp32 -g -o hello_riscv_pk_ecall.elf hello_riscv_shwetank.c`
     (Note: No `--specs=nosys.specs` or `--specs=nano.specs`)
   - Outcome: Successfully compiled and linked. Produced `hello_riscv_pk_ecall.elf`.
   - ELF Verification (`file hello_riscv_pk_ecall.elf`):
     `hello_riscv_pk_ecall.elf: ELF 32-bit LSB executable, UCB RISC-V, RVC, soft-float ABI, version 1 (SYSV), statically linked, with debug_info, not stripped` - Confirmed valid executable.

Key Learnings from Task 2:
- The xPack toolchain can successfully compile and link C programs for `rv32imac ilp32`.
- The use of `--specs=nosys.specs` stubs out syscalls, preventing `printf` output in environments like `pk` that expect actual `ecall` instructions.
- Compiling *without* `nosys.specs` allows `newlib` (the C library) to generate these `ecall`s, enabling `pk` to proxy I/O.
- Adding `-g` is crucial for including debug symbols for GDB.

Status for Task 2: COMPLETED.
