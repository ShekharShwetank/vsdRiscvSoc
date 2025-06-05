################################################################################
# Task 5: ABI & Register Cheat-Sheet
# Author: Shwetank
# Date: June 4, 2025
# Program: India RISC-V Chip Tapeout
# Phase 1, Week 1
################################################################################

Objective:
To document and understand the 32 RV32 integer registers, their ABI names,
and their roles in the standard RISC-V calling convention.

--- RV32 Integer Register Table ---
Register	ABI Name	Role	Saved By (Preserved across calls)
x0	zero	Hardwired to 0; writes are ignored	N/A (immutable)
x1	ra	Return Address	Caller
x2	sp	Stack Pointer	Callee
x3	gp	Global Pointer	N/A (special handling)
x4	tp	Thread Pointer	N/A (special handling)
x5	t0	Temporary / Caller-Saved Register 0	Caller
x6	t1	Temporary / Caller-Saved Register 1	Caller
x7	t2	Temporary / Caller-Saved Register 2	Caller
x8	s0/fp	Saved Register 0 / Frame Pointer	Callee
x9	s1	Saved Register 1	Callee
x10	a0	Function Argument 0 / Return Value 0	Caller
x11	a1	Function Argument 1 / Return Value 1	Caller
x12	a2	Function Argument 2	Caller
x13	a3	Function Argument 3	Caller
x14	a4	Function Argument 4	Caller
x15	a5	Function Argument 5	Caller
x16	a6	Function Argument 6	Caller
x17	a7	Function Argument 7	Caller
x18	s2	Saved Register 2	Callee
x19	s3	Saved Register 3	Callee
x20	s4	Saved Register 4	Callee
x21	s5	Saved Register 5	Callee
x22	s6	Saved Register 6	Callee
x23	s7	Saved Register 7	Callee
x24	s8	Saved Register 8	Callee
x25	s9	Saved Register 9	Callee
x26	s10	Saved Register 10	Callee
x27	s11	Saved Register 11	Callee
x28	t3	Temporary / Caller-Saved Register 3	Caller
x29	t4	Temporary / Caller-Saved Register 4	Caller
x30	t5	Temporary / Caller-Saved Register 5	Caller
x31	t6	Temporary / Caller-Saved Register 6	Caller

--- Calling Convention Summary ---
Argument Passing (a0-a7):

    The first eight integer/pointer arguments to a function are passed in registers a0 through a7.
    If there are more than eight arguments, or if arguments are large structures, they are passed on the stack.

Return Values (a0, a1):

    The primary return value (integer or pointer) is placed in register a0.
    If a function returns a 64-bit integer on a 32-bit system (RV32), a1 is used for the upper 32 bits. For larger return values (like structs), they are typically returned via a pointer passed as an implicit first argument.

Caller-Saved Registers (Temporary Registers: ra, t0-t6, a0-a7):

    A function (the "caller") that calls another function (the "callee") cannot assume these registers will hold their values after the callee returns.
    If the caller needs the value in a temporary register after a function call, the caller must save it to the stack before the call and restore it afterwards.
    a0-a7 are caller-saved because they are used for arguments and return values, so their original content is naturally overwritten.

Callee-Saved Registers (Saved Registers: sp, s0-s11):

    A function (the "callee") must ensure that these registers have the same values when it returns to its caller as they had when the callee was called.
    If the callee needs to use any of these saved registers, it must first save their original values onto the stack (typically in its prologue) and then restore them before returning (in its epilogue). We saw this with s0 in our Task 3 assembly.
    sp (stack pointer) is callee-saved; while a function modifies sp to allocate its own stack frame, it must restore sp to its original value before returning (effectively deallocating its frame).

Special Registers:

    zero (x0): Always reads as 0. Writes to it are ignored.
    gp (x3 - Global Pointer): Used to access global variables more efficiently. Its setup and usage are part of the ABI and linker's responsibility. It's typically assumed to be preserved by convention, but its value is set at program startup.
    tp (x4 - Thread Pointer): Used in multi-threaded environments to point to thread-local storage.

Key Takeaways:
- Understanding which registers are for arguments (a0-a7) and return values (a0, a1).
- Differentiating between caller-saved (t0-t6, a0-a7, ra) and callee-saved (s0-s11, sp) registers is critical for correct function calls and register usage in assembly.
- s0 is often used as the frame pointer (fp).

Status for Task 5: COMPLETED.
