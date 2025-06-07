/*
 * File: interrupt_linker.ld
 * Author: Shwetank
 * Date: June 6, 2025
 * Task: p1w1 - Task 13: Interrupt Primer
 * Purpose: A linker script for a bare-metal interrupt demo.
 * Places .text at 0x80000000 (a common RAM start for emulators)
 * and defines a stack region.
 */

OUTPUT_ARCH(riscv)
ENTRY(_start)

MEMORY
{
  RAM (rwx) : ORIGIN = 0x80000000, LENGTH = 64M
}

SECTIONS
{
  .text : {
    KEEP(*(.text.init)) /* Keep our startup code */
    *(.text*)
    *(.rodata*)
  } > RAM

  . = ALIGN(8);
  PROVIDE(__global_pointer$ = .);

  .data : {
    *(.data*)
    *(.sdata*)
  } > RAM

  .bss : {
    _sbss = .;
    *(.bss*)
    *(.sbss*)
    *(COMMON)
    _ebss = .;
  } > RAM

  /* Define the stack region */
  /* The stack grows downwards from the top of RAM */
  .stack (NOLOAD) : {
    . = ALIGN(16);
    _stack_top = ORIGIN(RAM) + LENGTH(RAM); /* Top of RAM */
    . = _stack_top;
    _stack_bottom = .; /* Symbol for the bottom of stack space */
  } > RAM

  _end = .; /* Symbol for the end of our used data */
}
