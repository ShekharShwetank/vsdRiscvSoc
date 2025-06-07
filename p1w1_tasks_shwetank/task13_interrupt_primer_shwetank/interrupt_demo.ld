/*
 * File: interrupt_demo.ld
 * Author: Shwetank
 * Purpose: Linker script for bare-metal interrupt demo on QEMU virt machine.
 */
OUTPUT_ARCH(riscv)
ENTRY(_start)

MEMORY
{
  /* QEMU 'virt' machine's main RAM */
  RAM (rwx) : ORIGIN = 0x80000000, LENGTH = 128M
}

SECTIONS
{
  .text : {
    *(.text.entry) /* Ensure our _start code is first */
    *(.text*)
  } >RAM

  .rodata : {
    . = ALIGN(4);
    *(.rodata*)
  } >RAM

  .data : {
    . = ALIGN(4);
    *(.data*)
  } >RAM

  .bss : {
    . = ALIGN(4);
    __bss_start = .;
    *(.bss*)
    *(COMMON)
    __bss_end = .;
  } >RAM
  
  /* Define a stack region at the end of RAM.
     This region is not loaded from the ELF, just space reserved. */
  .stack (NOLOAD) : {
    . = ALIGN(16);
    . = . + 4K; /* Allocate a 4 KB stack */
    _stack_top = .; /* Define a symbol for the top of the stack */
  } >RAM
}
