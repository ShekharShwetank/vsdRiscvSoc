# File: start.S
# Author: Shwetank
# Purpose: Minimal bare-metal startup code for interrupt demo.

.section .text.entry
.option norvc # Disable compressed instructions for simplicity in startup
.globl _start

_start:
    # 1. Initialize Stack Pointer (sp).
    # We load the address of the symbol _stack_top, which will be defined
    # at the top of a stack region by our linker script.
    la sp, _stack_top

    # 2. Set the Machine Trap Vector (mtvec) to point to our handler.
    # The C function `m_timer_interrupt_handler` is our handler.
    # The attribute((interrupt)) ensures it's a valid trap handler entry point.
    la t0, m_timer_interrupt_handler
    csrw mtvec, t0

    # 3. Enable Machine Timer Interrupts specifically.
    # We set the MTIE bit (bit 7) in the 'mie' CSR.
    li t0, (1 << 7)  # t0 = 0b10000000
    csrw mie, t0

    # 4. Enable Interrupts Globally.
    # We set the MIE bit (bit 3) in the 'mstatus' CSR.
    # `csrs` is an atomic read-and-set instruction.
    li t0, (1 << 3)  # t0 = 0b1000
    csrs mstatus, t0

    # 5. Call our main C function.
    call main

# If main somehow returns, trap the processor in an infinite loop.
_halt:
    j _halt
