################################################################################
# Task 4: Hex Dump & Disassembly
# Author: Shwetank
# Date: June 5, 2025
# Program: India RISC-V Chip Tapeout
# Phase 1, Week 1
################################################################################

Objective:
To learn how to generate a disassembly listing and an Intel HEX file from an ELF executable (`hello_riscv_xpack.elf` - the version compiled with --specs=nosys.specs and --specs=nano.specs was used for this task, though `hello_riscv_pk_ecall.elf` could also be used) and to understand the components of a disassembled instruction line.

Setup:
- Toolchain: xPack GNU RISC-V Embedded GCC v14.2.0-3 (riscv-none-elf-objdump, riscv-none-elf-objcopy).
- Input File: hello_riscv_xpack.elf (copied from Task 2 directory).
- Workspace: ~/riscv_tapeout_shwetank_workspace/p1w1_tasks_shwetank/task4_hexdump_disassembly_shwetank/

Commands Executed:

1. Disassembly:
   riscv-none-elf-objdump -d hello_riscv_xpack.elf > hello_riscv_xpack_task4.dump
   - Purpose: To get a human-readable assembly listing from the compiled ELF.
   - Output: `hello_riscv_xpack_task4.dump`.

2. Hex File Generation (Intel HEX format):
   riscv-none-elf-objcopy -O ihex hello_riscv_xpack.elf hello_riscv_xpack_task4.hex
   - Purpose: To convert the ELF binary into Intel HEX format, often used for programming microcontrollers or for some simulation/verification tools.
   - Output: `hello_riscv_xpack_task4.hex`.

Understanding `objdump -d` Output Columns:

Example Line from `hello_riscv_xpack_task4.dump` (illustrative, from `main`):
  `   1015c:	1141          	addi	sp,sp,-16`

Breakdown:
- Address: `1015c` (hexadecimal)
  - The memory location (or offset within the section) of this instruction.
- Opcode (Machine Code): `1141` (hexadecimal)
  - The raw instruction bytes as stored in the ELF file. In this case, `1141` represents a 2-byte compressed instruction `c.addi sp, -16`. (A 4-byte instruction would have 8 hex digits, e.g., `f14022f3`).
- Mnemonic: `addi` (or `c.addi` if objdump shows the compressed form explicitly)
  - The symbolic name of the operation (Add Immediate).
- Operands: `sp,sp,-16`
  - The registers and immediate values the instruction acts upon.
    - Destination: `sp`
    - Source(s)/Immediates: `sp`, `-16`

Status for Task 4: COMPLETED.
