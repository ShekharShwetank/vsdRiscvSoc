################################################################################
# Task 1: Install & Sanity-Check the RISC-V Toolchain
# Author: Shwetank
# Date: June 5, 2025
# Program: India RISC-V Chip Tapeout
# Phase 1, Week 1
################################################################################

Objective:
To install a functional RISC-V cross-compiler toolchain (targeting rv32imac, ilp32 ABI, bare-metal ELF) on Ubuntu 24.04 LTS. This involved adding it to the system PATH and performing sanity checks, critically verifying its ability to compile assembly code with CSR (Control and Status Register) instructions and C code using standard libraries.

Initial Workspace Setup:
- A new workspace was established at: ~/riscv_tapeout_shwetank_workspace/
- Subdirectories created: toolchain_riscv_shwetank/, p1w1_tasks_shwetank/, riscv_tools_src_shwetank/, documentation_shwetank/.

Toolchain Installation Journey & Learnings:

1. Initial Program-Guided Toolchain Attempt:
   - Tool: `riscv-toolchain-rv32imac-x86_64-ubuntu.tar.gz` (understood from initial program materials `p1w1.pdf`).
   - Installation Path: Extracted by user to `~/riscv_shwetank/opt/riscv/`.
   - PATH Configuration: Added `$HOME/riscv_shwetank/opt/riscv/bin` to `~/.bashrc`.
   - Initial Issues & Fixes:
     - GDB `libpython3.10.so.1.0` missing: Resolved by installing `libpython3.10-dev` package. `gcc`, `objdump`, `gdb` were then initially verified.
     - GCC Multilib: `riscv32-unknown-elf-gcc --print-multi-lib` output was `.;`. C code (e.g., "Hello, World!") failed to compile with `-march=rv32imc` due to "Cannot find suitable multilib set". Workaround: C code compiled successfully using `-march=rv32imac`.
     - GDB `target sim`: Proved highly unreliable for the compiled ELF (`hello_riscv_shwetank_t2_output.elf`). Issues included "No program loaded," immediate `SIGILL, Illegal instruction` on `run` (even with breakpoints at `_start`), GDB reporting "The program has no registers now," and inability to `set $pc`.
     - OpenSBI/Proxy Kernel Build Failures:
       - OpenSBI v1.4 build attempt (for QEMU) failed: Linker (`ld.bfd`) reported `unrecognized option '--exclude-libs'`.
       - OpenSBI `master` branch build attempt (for QEMU) failed: Linker reported `Your linker does not support creating PIEs, opensbi requires this.`. A test confirmed the linker does not support `-pie` (`ld: -pie not supported`).
       - RISC-V Proxy Kernel (`pk`) build attempt (for Spike) failed: Assembler reported `Error: unrecognized opcode 'csrr...', extension 'zicsr' required` for multiple standard CSR instructions.
   - **CRITICAL FAILURE (Toolchain Assembler Integrity):**
     - A direct test using a minimal assembly file (`test_csr.S`) with a `csrr` instruction failed to assemble with both `-march=rv32imac` and `-march=rv32i`.
       - Command: `riscv32-unknown-elf-gcc -march=rv32i -mabi=ilp32 -o test_csr_rv32i.elf test_csr.S -nostdlib -nostartfiles -Ttext=0x0`
       - Error: `test_csr.S:4: Error: unrecognized opcode 'csrr t0,mhartid', extension 'zicsr' required`.
   - Reporting: The severity of this toolchain issue (inability to assemble fundamental CSR instructions) prompted the drafting of an email to the program manager.
   - Conclusion: This initial toolchain was deemed unsuitable and fundamentally flawed for the program tasks due to its inability to correctly assemble standard CSR instructions necessary for low-level software development (like `pk` or OpenSBI) and its linker incompatibilities.

2. Attempt with `riscv-collab` Nightly Build:
   - Tool: `riscv32-elf-ubuntu-24.04-gcc-nightly-2025.05.30-nightly.tar.xz`.
   - Issues:
     - `--print-multi-lib` output was `.;`, indicating multilib issues similar to the initial toolchain.
     - Required `-march=rv32imac_zicsr` for basic CSR assembly (indicating `zicsr` was not implicitly enabled by `rv32imac` for the assembler).
     - CRITICAL FAILURE: Could not link C programs using `printf` (e.g., "Hello, World!") due to "Cannot find suitable multilib set" errors for `rv32imac` and `rv32imac_zicsr` targets when standard libraries were involved.
   - Conclusion: Deemed unsuitable for C development tasks requiring standard libraries. The directory was renamed to `riscv_nightly_problematic_20250530`.

3. Successful Installation
   - Rationale: Following mentor's advice to "figure it out,it  is all a learning process"; riscv-none-elf-gcc-14.2.0-3-linux-x64 was chosen as a alternative.
   - Tool Downloaded: `riscv-none-elf-gcc-14.2.0-3-linux-x64.tar.gz`.
   - Installation Path: Extracted to `~/riscv_tapeout_shwetank_workspace/toolchain_riscv_shwetank/riscv-none-elf-gcc-14.2.0-3/`.
   - PATH Configuration: Updated `~/.bashrc` to include:
     `export PATH=$HOME/riscv_tapeout_shwetank_workspace/toolchain_riscv_shwetank/riscv-none-elf-gcc-14.2.0-3/bin:$PATH`
     Previous toolchain paths were removed/commented.
   - Verification:
     - `which riscv-none-elf-gcc`: Confirmed correct path.
     - Versions: GCC 14.2.0, objdump 2.43.1, GDB 15.1.
     - `riscv-none-elf-gcc --print-multi-lib`: Successfully listed numerous multilib configurations (e.g., `rv32imc/ilp32`), indicating proper library support for C programs. This was a key positive indicator.
   - CSR Assembly Test (`test_csr_shwetank.S`):
     - File created in `task1_toolchain_csr_test_shwetank/` with basic `csrr` instructions.
     - Compilation with `-march=rv32imac` FAILED ("unrecognized opcode... zicsr required").
     - Compilation with `-march=rv32imac_zicsr` SUCCEEDED:
       `riscv-none-elf-gcc -march=rv32imac_zicsr -mabi=ilp32 -nostdlib -nostartfiles -Ttext=0x0 -o test_csr_zicsr.elf test_csr_shwetank.S`
     - `riscv-none-elf-objdump -d test_csr_xpack_zicsr.elf` confirmed correct CSR instruction disassembly.

Final Working Toolchain for Subsequent Tasks:
- Executable prefix: `riscv-none-elf-`.
- Important Note: For assembly involving CSRs, `-march=rv32imac_zicsr` (or a similar explicit enabling of `_zicsr`) is required with this toolchain. For C code using standard libraries, `-march=rv32imc` or `-march=rv32imac` should work due to good multilib support.

Key Learnings from Task 1:
- Toolchain selection and verification are paramount. Default configurations of even official/nightly builds can have limitations (e.g., multilib setup, implicit ISA extension enablement for the assembler).
- The `--print-multi-lib` GCC option is a valuable diagnostic for C library support.
- For some toolchains, ISA sub-extensions (like `_zicsr`, `_zifencei`) must be explicitly added to the `-march` string for the assembler to recognize all standard instructions, even if the base ISA version implies them by specification. This was observed with the latest toolchain for CSRs in assembly.
- later toolchains demonstrated robust multilib support for C code, which was lacking in the other attempts.
- The initial program-guided toolchain and the `riscv-toolchain-rv32imac-x86_64-ubuntu.tar.gz` was found to be fundamentally flawed in its CSR assembly capabilities for `rv32i/imac`.

Status for Task 1: COMPLETED
