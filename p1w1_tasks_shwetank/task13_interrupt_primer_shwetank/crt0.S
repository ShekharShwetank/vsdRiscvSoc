#
# File: crt0.S
# Author: Shwetank
# Date: June 7 , 2025
# Task: p1w1 - Task 13: Interrupt Primer
# Purpose: Minimal startup code for a bare-metal RISC-V program.
#

.section .text.init
.globl _start

_start:
    # 1. Initialize the Stack Pointer (sp)
    # The linker script provides the _stack_top symbol.
    la sp, _stack_top

    # 2. Initialize the Global Pointer (gp)
    # The linker automatically calculates the correct gp value.
    # The 'la gp, __global_pointer$' pseudo-instruction handles this.
    # The linker will relax this into the correct auipc/addi pair.
    .option push
    .option norelax
    la gp, __global_pointer$
    .option pop

    # 3. Clear the .bss section
    # The linker script provides symbols _sbss and _ebss.
    la a0, _sbss
    la a1, _ebss
    bgeu a0, a1, .bss_clear_done

.bss_clear_loop:
    sw zero, 0(a0)
    addi a0, a0, 4
    blt a0, a1, .bss_clear_loop

.bss_clear_done:

    # 4. Call main()
    call main

    # 5. Handle main's return: Enter an infinite loop.
    # A bare-metal program should not exit.
_exit_loop:
    wfi # Wait for an interrupt to save power
    j _exit_loop
