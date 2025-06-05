################################################################################
# Task 3: From C to Assembly
# Author: Shwetank
# Date: June 5, 2025
# Program: India RISC-V Chip Tapeout
# Phase 1, Week 1
################################################################################

Objective:
To generate an assembly (.s) file from the "Hello, RISC-V" C code (hello_riscv_shwetank.c) and analyze the prologue and epilogue of the main function.

Setup:
- Toolchain: xPack GNU RISC-V Embedded GCC v14.2.0-3 (riscv-none-elf-gcc).
- C Source File: hello_riscv_shwetank.c (copied from Task 2 directory).
- Workspace: ~/riscv_tapeout_shwetank_workspace/p1w1_tasks_shwetank/task3_c_to_assembly_shwetank/

Assembly Generation:
- Command Used:
  riscv-none-elf-gcc -S -O0 -march=rv32imac -mabi=ilp32 -o hello_riscv_shwetank_task3.s hello_riscv_shwetank.c
- `-S`: Instructs GCC to output an assembly file.
- `-O0`: Specifies no optimization, for clearer, more standard assembly.
- Output File: hello_riscv_shwetank_task3.s

Analysis of `main` Function's Prologue & Epilogue in `hello_riscv_shwetank_task3.s`:

Generated Assembly for `main`:
  main:
      addi    sp,sp,-16     # Prologue: Allocate 16 bytes on stack
      sw      ra,12(sp)     # Prologue: Save return address (ra)
      sw      s0,8(sp)      # Prologue: Save frame pointer (s0)
      addi    s0,sp,16      # Prologue: Set new frame pointer (s0 = old sp)
      lui     a5,%hi(.LC0)  # Load upper immediate for string address
      addi    a0,a5,%lo(.LC0)# Load full string address into a0 (arg for puts)
      call    puts          # Call puts (optimized from printf)
      li      a5,0          # Load 0 into a5 (return value)
      mv      a0,a5         # Move 0 to a0 (return register)
      lw      ra,12(sp)     # Epilogue: Restore return address
      lw      s0,8(sp)      # Epilogue: Restore frame pointer
      addi    sp,sp,16      # Epilogue: Deallocate stack space
      jr      ra            # Epilogue: Return (jump to ra)

Key Observations:
- Prologue:
  - Stack pointer (`sp`) is decremented by 16 to allocate space.
  - Return address (`ra`) and frame pointer (`s0`) are saved onto the stack. `ra` is saved because `call puts` will overwrite it. `s0` is saved as it's a callee-saved register used as a frame pointer.
  - `s0` is set to point to the base of the new stack frame.
- Epilogue:
  - `ra` and `s0` are restored from the stack.
  - Stack space is deallocated by incrementing `sp`.
  - `jr ra` (jump to `ra`) returns execution to the caller.
- `printf` was optimized to `puts` by the compiler for simple string output.

Status for Task 3: COMPLETED.
