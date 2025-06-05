################################################################################
# Task 8: Exploring GCC Optimisation
# Author: Shwetank
# Date: June 5, 2025
# Program: India RISC-V Chip Tapeout
# Phase 1, Week 1
################################################################################

Objective:
To compile the same C file (hello_riscv_shwetank.c) with different GCC
optimization levels (-O0 and -O2), generate the assembly for both,
and analyze the differences in the generated code.

Setup:
- Toolchain: xPack GNU RISC-V Embedded GCC v14.2.0-3 (riscv-none-elf-gcc).
- C Source File: hello_riscv_shwetank.c.
- Workspace: ~/riscv_tapeout_shwetank_workspace/p1w1_tasks_shwetank/task8_gcc_optimisation_shwetank/

Assembly Generation Commands:
1. No Optimisation (-O0):
   riscv-none-elf-gcc -S -O0 -march=rv32imac -mabi=ilp32 -o hello_O0_shwetank.s hello_riscv_shwetank.c
   - Output: hello_O0_shwetank.s

2. Level 2 Optimisation (-O2):
   riscv-none-elf-gcc -S -O2 -march=rv32imac -mabi=ilp32 -o hello_O2_shwetank.s hello_riscv_shwetank.c
   - Output: hello_O2_shwetank.s

Comparison and Analysis of `main` Function:

1. No Optimisation (-O0) from hello_O0_shwetank.s:
Code snippet

main:
    addi    sp,sp,-16       # Prologue: Allocate 16 bytes on stack
    sw      ra,12(sp)       # Prologue: Save return address (ra)
    sw      s0,8(sp)        # Prologue: Save frame pointer (s0)
    addi    s0,sp,16        # Prologue: Set new frame pointer s0
    lui     a5,%hi(.LC0)    # Load upper immediate of string address into a5
    addi    a0,a5,%lo(.LC0) # Add lower immediate to a5, result in a0 (arg for puts)
    call    puts            # Call puts
    li      a5,0            # Load immediate 0 into a5
    mv      a0,a5           # Move 0 from a5 to a0 (return value)
    lw      ra,12(sp)       # Epilogue: Restore ra
    lw      s0,8(sp)        # Epilogue: Restore s0
    addi    sp,sp,16        # Epilogue: Deallocate stack
    jr      ra              # Epilogue: Return

2. Level 2 Optimisation (-O2) from hello_O2_shwetank.s:
Code snippet

main:
    lui     a0,%hi(.LC0)    # Load upper immediate of string address directly into a0
    addi    sp,sp,-16       # Prologue: Allocate 16 bytes on stack
    addi    a0,a0,%lo(.LC0) # Add lower immediate to a0, result in a0 (arg for puts)
    sw      ra,12(sp)       # Prologue: Save return address (ra)
    call    puts            # Call puts
    lw      ra,12(sp)       # Epilogue: Restore ra
    li      a0,0            # Load immediate 0 directly into a0 (return value)
    addi    sp,sp,16        # Epilogue: Deallocate stack
    jr      ra              # Epilogue: Return

Key Differences Observed:

1.  Frame Pointer (`s0`) Usage:
    - `-O0`: Explicitly saves `s0` (used as a frame pointer), sets it up (`addi s0,sp,16`), and restores it. This is standard unoptimized behavior for stack frame management.
    - `-O2`: Completely eliminates the use of `s0` as a dedicated frame pointer. Instructions `sw s0,8(sp)`, `addi s0,sp,16`, and `lw s0,8(sp)` are absent. The compiler determined it could manage stack accesses relative to `sp` directly for this simple function. This saves 3 instructions and frees up `s0`.

2.  String Address Loading & Argument Passing:
    - `-O0`: Uses `a5` as an intermediate register to construct the address of the string `.LC0` before moving it to `a0` (the argument register for `puts`).
      `lui a5,%hi(.LC0)`
      `addi a0,a5,%lo(.LC0)`
    - `-O2`: Loads the address of `.LC0` directly into `a0`.
      `lui a0,%hi(.LC0)`
      `addi a0,a0,%lo(.LC0)`
      This is more direct and avoids using `a5` unnecessarily for this step.

3.  Return Value Setup:
    - `-O0`: Loads `0` into `a5` first, then moves it to `a0`.
      `li a5,0`
      `mv a0,a5`
    - `-O2`: Directly loads `0` into `a0`.
      `li a0,0`
      This saves one `mv` instruction.

4.  Instruction Reordering:
    - In `-O2`, `addi sp,sp,-16` (stack allocation) is done *after* the `lui a0,%hi(.LC0)` instruction, whereas in `-O0` it's the first instruction. The optimizer can reorder instructions if dependencies allow, potentially for better performance (though minimal impact here).

5.  No Inlining of `puts`:In both cases, an explicit `call puts` instruction is present, meaning `puts` was not inlined into `main` by either `-O0` or `-O2` for this specific case and compiler version.

Reasons for Differences (GCC Optimisations Illustrated):
- Frame Pointer Elimination:** `-O2` decided a frame pointer wasn't necessary.
- More Efficient Register Use / Reduced Register Pressure:** `-O2` used `a0` more directly for constructing the string address.
- Instruction Elimination:** The `mv` instruction for the return value was eliminated in `-O2`.

This analysis demonstrates that even for a simple "Hello, World!" program, `-O2` makes the code more compact and potentially faster by removing redundant operations and using registers more effectively.

Status for Task 8: COMPLETED.

