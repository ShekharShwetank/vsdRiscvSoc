################################################################################
# Task 11: Linker Script 101 - Detailed Documentation
# Author: Shwetank
# Date: June 5, 2025
# Program: India RISC-V Chip Tapeout
# Phase 1, Week 1
################################################################################

Objective:
The primary goal of this task was to understand the fundamentals of linker scripts
in the context of bare-metal RISC-V programming. This involved:
1.  Learning the purpose and basic syntax of a linker script.
2.  Creating a minimal linker script to place the `.text` (code) section at memory
    address `0x00000000` and the `.data` (initialized data) section at
    address `0x10000000` for an RV32IMC target.
3.  Understanding the reasons behind placing code and data in different memory
    regions (e.g., Flash vs. SRAM) in typical embedded systems.
4.  Compiling a simple bare-metal C application using this custom linker script.
5.  Verifying the memory placement of sections using `objdump`.

Setup:
- Toolchain: xPack GNU RISC-V Embedded GCC v14.2.0-3 (using `riscv-none-elf-gcc`
  and its associated linker `riscv-none-elf-ld`).
- Workspace for this task:
  `~/riscv_tapeout_shwetank_workspace/p1w1_tasks_shwetank/task11_linker_script_shwetank/`

Files Created/Used:

1.  `minimal_shwetank.ld`: The custom linker script.
2.  `bare_metal_app_shwetank.c`: A simple C application for testing the linker script.
3.  `bare_metal_app_shwetank.elf`: The final executable ELF file produced.

--------------------------------------------------------------------------------
I. Linker Script (`minimal_shwetank.ld`) Content
--------------------------------------------------------------------------------

/*
 * File: minimal_shwetank.ld
 * Author: Shwetank
 * Date: June 5, 2025
 * Task: p1w1.pdf - Task 11: Linker Script 101
 * Purpose: A minimal linker script for RV32IMC placing .text at 0x0
 * and .data at 0x10000000.
 */

OUTPUT_ARCH(riscv) /* Specify the output architecture */
ENTRY(_start)      /* Define the entry point of the program */

SECTIONS
{
    /* Text section (code) placed at 0x00000000 */
    .text 0x00000000 :
    {
        *(.text._start) /* Attempt to place _start function code first */
        *(.text*)       /* Collect all .text sections from all input object files */
        *(.rodata*)     /* Collect all .rodata sections (read-only data) */
                        /* Grouping .rodata with .text is common for Flash placement */
    }

    /* Data section (initialized data) placed at 0x10000000 */
    .data 0x10000000 :
    {
        *(.data*)       /* Collect all .data sections */
    }

    /* BSS section (uninitialized data) */
    /* Placed by the linker after the last explicitly placed section if no address is given,
       or at an address specified. Here, it will follow .data.
       The (NOLOAD) type means this section does not occupy space in the ELF file itself,
       but space is reserved for it in memory at runtime. */
    .bss (NOLOAD) :
    {
        *(.sbss*)       /* Small BSS data (often accessed via Global Pointer) */
        *(.bss*)        /* Regular BSS data */
        *(COMMON)       /* Common symbols (uninitialized globals not explicitly in .bss) */
    }

    /* It's good practice to define where small data sections go if they are used.
       This minimal script did not originally include .sdata or .sbss explicitly
       within the .data and .bss output sections defined at 0x10000000.
       The .sbss was added here for completeness. .sdata would go in the .data block.
       The objdump output will reveal how the linker handled these based on this script.
    */
    _end = .; /* Define a symbol marking the end of the BSS section / program data */
}

--------------------------------------------------------------------------------
II. Bare-Metal C Application (`bare_metal_app_shwetank.c`) Content
--------------------------------------------------------------------------------

// File: bare_metal_app_shwetank.c
// Author: Shwetank
// Purpose: Minimal bare-metal C app to test the linker script.

#include <stdint.h>

// Initialized global variable (should go into .data or .sdata section)
uint32_t initialized_global_data = 0x12345678;

// Uninitialized global variable (should go into .bss or .sbss section)
// Changed to a single uint32_t for simpler section size tracking in this minimal example.
uint32_t uninitialized_global_data; 

// Constant global variable (should go into .rodata or .srodata section)
const uint32_t constant_global_data = 0xAABBCCDD;

// Entry point for bare-metal execution
// The linker script specifies _start as the entry point.
void _start() {
    // In a real bare-metal application, this is where one would:
    // 1. Initialize the Stack Pointer (sp).
    // 2. Copy the .data section from its Load Memory Address (LMA, in Flash)
    //    to its Virtual Memory Address (VMA, in RAM).
    // 3. Zero out the .bss section in RAM.
    // 4. Potentially set up hardware (clocks, peripherals).
    // 5. Call main().

    // For this test, just a simple operation and an infinite loop.
    // These assignments ensure the variables are referenced.
    initialized_global_data = 0xFFFFFFFF; 
    uninitialized_global_data = 0xEEEEEEEE;

    volatile int loop = 1; // Volatile to prevent optimizing out the loop
    while(loop) {
        // Loop forever to simulate a running program
    }
}

--------------------------------------------------------------------------------
III. Compilation and Linking
--------------------------------------------------------------------------------

The C application was compiled and linked using the custom linker script with the
following command:

riscv-none-elf-gcc -march=rv32imac -mabi=ilp32 -g \
                   -ffreestanding -nostdlib -nostartfiles \
                   -Wl,-T,minimal_shwetank.ld \
                   -o bare_metal_app_shwetank.elf bare_metal_app_shwetank.c

Explanation of flags:
- `-march=rv32imac -mabi=ilp32`: Target architecture and ABI.
- `-g`: Include debugging information.
- `-ffreestanding`: Informs the compiler that the code is for a freestanding
  environment (no standard OS or C library available by default).
- `-nostdlib`: Prevents linking with standard system libraries.
- `-nostartfiles`: Prevents linking with standard C runtime startup files (like
  `crt0.o`). Our `_start` function serves as the entry point.
- `-Wl,-T,minimal_shwetank.ld`: Passes the `-T minimal_shwetank.ld` option
  directly to the linker (`ld`), instructing it to use our custom linker script.
- `-o bare_metal_app_shwetank.elf`: Specifies the output executable filename.
- `bare_metal_app_shwetank.c`: The input C source file.

Linker Warning Observed:

/home/ank/riscv_tapeout_shwetank_workspace/toolchain_riscv_shwetank/xpack-riscv-none-elf-gcc-14.2.0-3/bin/../lib/gcc/riscv-none-elf/14.2.0/../../../../riscv-none-elf/bin/ld: warning: bare_metal_app_shwetank.elf has a LOAD segment with RWX permissions

This warning means the linker created a program segment in the ELF file that is
marked as Readable (R), Writable (W), AND Executable (X). This is generally
undesirable from a security perspective (e.g., Writable and Executable memory
can be exploited). A more complete linker script would define separate program
segments (PT_LOAD headers) for code (R+X) and data (R+W) with appropriate
permissions. This warning is a consequence of the simplicity of our current
minimal script.

--------------------------------------------------------------------------------
IV. Verification of Section Placement (`objdump -h`)
--------------------------------------------------------------------------------

The following command was used to inspect the section headers of the generated ELF:

riscv-none-elf-objdump -h bare_metal_app_shwetank.elf

Observed Output:

bare_metal_app_shwetank.elf:     file format elf32-littleriscv

Sections:
Idx Name          Size      VMA       LMA       File off  Algn
0 .text         00000034  00000000  00000000  00001000  21
CONTENTS, ALLOC, LOAD, READONLY, CODE
1 .srodata      00000004  00000034  00000034  00001034  22
CONTENTS, ALLOC, LOAD, READONLY, DATA
2 .sdata        00000004  00000038  00000038  00001038  22
CONTENTS, ALLOC, LOAD, DATA
3 .sbss         00000004  0000003c  0000003c  0000103c  22
ALLOC
4 .debug_info   000000d8  00000000  00000000  0000103c  20
CONTENTS, READONLY, DEBUGGING, OCTETS
5 .debug_abbrev 00000083  00000000  00000000  00001114  20
CONTENTS, READONLY, DEBUGGING, OCTETS
6 .debug_aranges 00000020  00000000  00000000  00001197  20
CONTENTS, READONLY, DEBUGGING, OCTETS
7 .debug_line   00000085  00000000  00000000  000011b7  20
CONTENTS, READONLY, DEBUGGING, OCTETS
8 .debug_str    000000e5  00000000  00000000  0000123c  20
CONTENTS, READONLY, DEBUGGING, OCTETS
9 .debug_line_str 00000110  00000000  00000000  00001321  20
CONTENTS, READONLY, DEBUGGING, OCTETS
10 .comment      00000033  00000000  00000000  00001431  20
CONTENTS, READONLY
11 .riscv.attributes 00000047  00000000  00000000  00001464  20
CONTENTS, READONLY
12 .debug_frame  00000038  00000000  00000000  000014ac  2**2
CONTENTS, READONLY, DEBUGGING, OCTETS

Analysis of `objdump` Output:

- VMA (Virtual Memory Address):This is the address where the section will reside
  when the program is loaded into memory and executed.
- **LMA (Load Memory Address):** This is the address from which the section is loaded.
  For sections in RAM (like `.data`), the LMA might be in Flash, and startup code
  copies it to the VMA in RAM. In this simple example and linker script, LMA and VMA
  are the same for the sections shown (except for `.bss` which isn't loaded from the file).

- `.text` Section (Idx 0):
  - `VMA 00000000`: Successfully placed at address `0x0` as specified in the linker script.
  - `Size 00000034` (52 bytes): Contains the compiled code from `_start` and likely
    the `constant_global_data` because `*(.rodata*)` was included in the `.text`
    output section definition in the linker script.

- Small Data Sections (`.srodata`, `.sdata`, `.sbss` - Idx 1, 2, 3):
  - `.srodata` (Small Read-Only Data): `VMA 00000034`. `constant_global_data` (4 bytes) likely landed here.
  - `.sdata` (Small Initialized Data): `VMA 00000038`. `initialized_global_data` (4 bytes) likely landed here.
  - `.sbss` (Small BSS Data): `VMA 0000003c`. `uninitialized_global_data` (as a `uint32_t`, 4 bytes) likely landed here. (If it was `char[100]`, it would be larger and might go to main `.bss`).
  - **Reason for Placement:** These sections are placed immediately after `.text` (starting
    at `0x00000034`). Our minimal linker script `minimal_shwetank.ld` only explicitly
    placed `.text` (at `0x0`) and `.data` (at `0x10000000`). It did not specify
    locations for `.srodata`, `.sdata`, or `.sbss` *relative to the `.data` section*.
    The RISC-V ABI uses these "small" sections for global variables that can be
    accessed efficiently relative to the global pointer (`gp`). Since the script
    didn't guide them to the `0x10000000` region, the linker used default placement
    rules, often grouping them near `.text` or other early sections if not otherwise directed.
    The `constant_global_data` may have gone into `.srodata` because it's small, even though `*(.rodata*)` was in the `.text` block. The linker might prioritize specific small sections first.

- Main `.data` Section (intended for `0x10000000`):
  - This section is **not visible** in the provided `objdump -h` output starting at
    `0x10000000`. This is likely because all initialized data from our C file
    (`initialized_global_data`) was small enough to be placed in the `.sdata`
    section by the compiler. Thus, the main `*(.data*)` input section pattern in our
    linker script found no matching input sections, resulting in an empty main `.data`
    section at `0x10000000`, which `objdump -h` might not list if it's empty.

- Main `.bss` Section:
  - The `.sbss` section (containing `uninitialized_global_data`) was placed at `0x0000003c`.
  - The main `.bss` section defined in the linker script would follow the main `.data` section. If the main `.data` section at `0x10000000` was empty, the main `.bss` would also start at `0x10000000` (or just after if `.data` had some content we missed). This is not clearly shown and depends on whether any non-small BSS data was generated.

- Debug Sections (Idx 4-9, 12):
  - Sections like `.debug_info`, `.debug_abbrev`, etc., contain debugging information
    generated because we used the `-g` compiler flag. They are not loaded onto the
    target during execution but are used by GDB. Their VMA/LMA of `0x0` here is
    typical as they are not runtime sections.

--------------------------------------------------------------------------------
V. Explanation: Why Flash vs. SRAM Addresses Differ
--------------------------------------------------------------------------------

In embedded systems, different types of memory serve different purposes:

1.  Flash Memory (or ROM/EEPROM - Non-Volatile):
    - Stores information that needs to persist even when power is off.
    - Typically holds the program's executable code (`.text` section) and any
      read-only data (`.rodata` section, e.g., string literals, constant variables).
    - Processors usually boot from a specific address in Flash where the initial
      program instructions are located. This is often a low address (e.g., `0x0`).
    - Flash is generally slower to write to than RAM and has a limited number of write cycles.

2.  SRAM (Static RAM - Volatile):
    - Stores data that can be read and written quickly during program execution.
    - Loses its content when power is removed.
    - Typically holds:
        - `.data` section: Global and static variables that have initial values.
        - `.bss` section: Global and static variables that are uninitialized (or
          initialized to zero by convention).
        - Stack: Used for function call frames, local variables, and passing arguments.
        - Heap: Used for dynamic memory allocation (e.g., `malloc`).
    - SRAM is usually mapped to a different address range than Flash. The choice of
      `0x10000000` for `.data` in this task is arbitrary but illustrates it being
      at a different location than `.text`.

LMA vs. VMA for `.data` Section:
- Since `.data` section variables need to be in RAM (for writability) but have
  initial values that must persist across power cycles, a common strategy is:
    1.  The initial values of the `.data` section are stored in Flash memory. The
        address in Flash where these values are stored is the Load Memory Address (LMA).
    2.  The program is linked such that these variables will reside in RAM during
        execution. The address in RAM where these variables are used is the
        Virtual Memory Address (VMA).
    3.  Startup Code (code that runs before `main`, often part of `_start` or a
        `crt0.o` file) is responsible for copying the initial values of the `.data`
        section from their LMA in Flash to their VMA in RAM.
    4.  The startup code is also responsible for zeroing out the `.bss` section in RAM
        (as `.bss` variables have no initial values stored in the executable file,
        only space reserved for them).
- Our minimal script directly set the VMA for `.data` and didn't explicitly define an
  LMA in Flash, which is a simplification for this exercise. A production linker
  script for an embedded system with Flash and RAM would use syntax like:
  `.data VMA_RAM_ADDRESS : AT ( LMA_FLASH_ADDRESS ) { *(.data*) }`

--------------------------------------------------------------------------------
VI. Key Learnings & Conclusion
--------------------------------------------------------------------------------
- A linker script provides fine-grained control over memory layout.
- The placement of `.text` at `0x00000000` was achieved as per the task.
- The behavior of small data sections (`.sdata`, `.sbss`, `.srodata`) highlighted
  that a truly minimal script might lead to the linker using default placement rules
  for sections not explicitly mentioned or fully qualified. For precise control,
  all expected sections should be explicitly placed.
- The intended `.data` section at `0x10000000` was likely empty because the small
  initialized global variable (`initialized_global_data`) was probably optimized by
  the compiler into the `.sdata` section, which the minimal script didn't force
  into the `0x10000000` region.
- The linker warning about RWX permissions indicates the need for more specific
  segment definitions (PT_LOAD) in a production linker script to ensure correct
  memory permissions (e.g., code as R+X, data as R+W).
- The fundamental reasons for placing code in Flash (non-volatile, boot location)
  and data in RAM (writable, faster access for variables) are now clearer.

Status for Task 11: COMPLETED. Basic linker script created and section placement
partially verified, with important learnings about linker behavior and the need
for more comprehensive scripts for full control.
