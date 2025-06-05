/*
 * File: minimal_shwetank.ld
 * Author: Shwetank
 * Date: June 5, 2025
 * Task: p1w1.pdf - Task 11: Linker Script 101
 * Purpose: A minimal linker script for RV32IMC placing .text at 0x0
 * and .data at 0x10000000.
 */

OUTPUT_ARCH(riscv) /* Specify the output architecture */
ENTRY(_start)      /* Define the entry point of the program, typically _start for bare-metal */

SECTIONS
{
    /* Text section (code) placed at 0x00000000 */
    .text 0x00000000 :
    {
        *(.text*)   /* Collect all .text sections from all input object files */
        *(.rodata*) /* It's common to place read-only data with text */
    }

    /* Data section (initialized data) placed at 0x10000000 */
    .data 0x10000000 :
    {
        *(.data*)   /* Collect all .data sections */
    }

    /* BSS section (uninitialized data) typically follows .data */
    /* This minimal example from p1w1.pdf doesn't explicitly place .bss,
       but a more complete script would. If not specified, the linker
       might place it after .data by default, or it might need explicit handling. */
    .bss :
    {
        *(.bss*)
        *(COMMON)
    }
}
