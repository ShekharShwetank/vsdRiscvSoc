##################
# Task 13: Interrupt Primer
# Author: Shwetank
# Date: June 7, 2025
# Program: India RISC-V Chip Tapeout
# Phase 1, Week 1
##################

### Objective:

To write a complete bare-metal RISC-V program that correctly initializes the C runtime environment, sets up a machine-level timer interrupt, and handles that interrupt in an Interrupt Service Routine (ISR). The goal was to verify that the ISR is executed periodically, demonstrating a fundamental aspect of embedded systems programming.

### Setup:

- **Toolchain:** xPack GNU RISC-V Embedded GCC v14.2.0-3 (`riscv-none-elf-`)
- **Emulator:** QEMU v8.2.2 (`qemu-system-riscv32`)
- **Debugger:** xPack GDB v15.1 (`riscv-none-elf-gdb`)
- **Files Created:**
    1.  `interrupt_linker.ld`: Custom linker script to define memory layout.
    2.  `crt0.S`: Minimal assembly startup code to initialize the C environment.
    3.  `interrupt_demo.c`: C application containing the main logic and the ISR.

### Execution and Verification Process:

The task involved building a bare-metal executable from scratch, which required careful orchestration of the linker, startup code, and the C application.

**1. Linker Script (`interrupt_linker.ld`):**
A custom linker script was created to map the program into the QEMU 'virt' machine's RAM, starting at `0x80000000`. It also defined symbols for the stack (`_stack_top`) and the BSS section (`_sbss`, `_ebss`), which are used by the startup code.

**2. Startup Code (`crt0.S`):**
A minimal assembly `crt0.S` was written to perform essential pre-main initializations:
- Set the stack pointer (`sp`) to the top of the defined stack region.
- Set the global pointer (`gp`) using the `__global_pointer$` symbol provided by the linker.
- Clear the `.bss` section by iterating from `_sbss` to `_ebss` and writing zeros.
- Call the `main` function.
- Enter an infinite `wfi` (Wait For Interrupt) loop if `main` ever returns.

**3. C Application (`interrupt_demo.c`):**
The C code implemented the core logic:
- An ISR `machine_timer_isr` was defined using the `__attribute__((interrupt("machine")))` directive. This tells GCC to generate the correct function prologue and epilogue for a machine-level interrupt handler (saving/restoring registers and using `mret`).
- Inside the ISR, the `mtimecmp` register is updated to schedule the next interrupt, and a global counter `interrupt_count` is incremented.
- The `main` function sets up the interrupt system by:
    a. Writing the ISR's address to the `mtvec` CSR.
    b. Setting the MTIE (Machine Timer Interrupt Enable) bit in the `mie` CSR.
    c. Scheduling the first interrupt by writing to the `mtimecmp` memory-mapped register.
    d. Setting the MIE (Machine Interrupt Enable) bit in the `mstatus` CSR to enable interrupts globally.
    e. Entering an infinite loop to wait for interrupts.

**4. Compilation Command:**
The program was compiled and linked with:
`riscv-none-elf-gcc -march=rv32imac_zicsr -mabi=ilp32 -g -ffreestanding -nostdlib -Wl,-T,interrupt_linker.ld -o interrupt_demo.elf crt0.S interrupt_demo.c`

**5. Verification with QEMU and GDB:**
- **QEMU Launch:** `qemu-system-riscv32 -nographic -machine virt -bios none -kernel interrupt_demo.elf -s -S`
- **GDB Session:**
    - `riscv-none-elf-gdb interrupt_demo.elf`
    - `(gdb) target remote localhost:1234`
    - `(gdb) break machine_timer_isr`
    - `(gdb) continue`
- **Observation:** The breakpoint in the ISR was hit repeatedly, and inspecting the `interrupt_count` variable showed it incrementing each time. This successfully verified that the timer interrupt was configured and handled correctly.

### Key Learnings and Challenges:

1.  **Linker Error (`undefined reference to \`__global_pointer$'`):** The initial compilation failed because `crt0.S` referenced the `__global_pointer$` symbol, but the linker script did not define it.
    - **Solution:** The linker script was fixed by adding `PROVIDE(__global_pointer$ = .);` to explicitly define the symbol at the start of the data sections.

2.  **QEMU Error (`Some ROM regions are overlapping`):** QEMU failed to launch the initial ELF file because the memory region claimed by our program (with `LENGTH = 128M`) conflicted with the region where QEMU loads its Flattened Device Tree (`fdt`).
    - **Solution:** The `LENGTH` in the `MEMORY` block of the linker script was reduced from `128M` to `64M`, resolving the memory map conflict.

3.  **Bare-Metal Dependencies:** This task highlighted how even a simple C program depends on a significant amount of underlying setup (stack, BSS clearing) that is normally hidden by the OS and standard libraries. Writing `crt0.S` and a linker script made this process explicit.

### Status for Task 13: COMPLETED.
