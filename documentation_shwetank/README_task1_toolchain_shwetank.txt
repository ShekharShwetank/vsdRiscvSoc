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

1. Attempt with `riscv-collab` Nightly Build (Summary):
   - Tool: `riscv32-elf-ubuntu-24.04-gcc-nightly-2025.05.30-nightly.tar.xz`.
   - Issues:
     - `--print-multi-lib` output was `.;`, indicating multilib issues.
     - Required `-march=rv32imac_zicsr` for basic CSR assembly.
     - CRITICAL FAILURE: Could not link C programs using `printf` (e.g., "Hello, World!") due to "Cannot find suitable multilib set" errors for `rv32imac` and `rv32imac_zicsr` targets.
   - Conclusion: Deemed unsuitable for C development tasks requiring standard libraries. The directory was renamed to `riscv_nightly_problematic_20250530`.

2. Successful Installation: xPack GNU RISC-V Embedded GCC (v14.2.0-3)
   - Rationale: Following mentor's advice, xPack was chosen as a well-maintained alternative.
   - Tool Downloaded: `xpack-riscv-none-elf-gcc-14.2.0-3-linux-x64.tar.gz` from [https://github.com/xpack-dev-tools/riscv-none-elf-gcc-xpack/releases/tag/v14.2.0-3](https://github.com/xpack-dev-tools/riscv-none-elf-gcc-xpack/releases/tag/v14.2.0-3).
   - Installation Path: Extracted to `~/riscv_tapeout_shwetank_workspace/toolchain_riscv_shwetank/xpack-riscv-none-elf-gcc-14.2.0-3/`.
   - PATH Configuration: Updated `~/.bashrc` to include:
     `export PATH=$HOME/riscv_tapeout_shwetank_workspace/toolchain_riscv_shwetank/xpack-riscv-none-elf-gcc-14.2.0-3/bin:$PATH`
     Previous toolchain paths were removed/commented.
   - Verification:
     - `which riscv-none-elf-gcc`: Confirmed correct path.
     - Versions: GCC 14.2.0, objdump 2.43.1, GDB 15.1.
     - `riscv-none-elf-gcc --print-multi-lib`: Successfully listed numerous multilib configurations (e.g., `rv32imc/ilp32`), indicating proper library support. This was a key positive indicator.
   - CSR Assembly Test (`test_csr_shwetank.S`):
     - File created in `task1_toolchain_csr_test_shwetank/` with basic `csrr` instructions.
     - Compilation with `-march=rv32imac` FAILED ("unrecognized opcode... zicsr required").
     - Compilation with `-march=rv32imac_zicsr` SUCCEEDED:
       `riscv-none-elf-gcc -march=rv32imac_zicsr -mabi=ilp32 -nostdlib -nostartfiles -Ttext=0x0 -o test_csr_xpack_zicsr.elf test_csr_shwetank.S`
     - `riscv-none-elf-objdump -d test_csr_xpack_zicsr.elf` confirmed correct CSR instruction disassembly.

Final Working Toolchain for Subsequent Tasks:
- xPack GNU RISC-V Embedded GCC v14.2.0-3.
- Executable prefix: `riscv-none-elf-`.

Key Learnings from Task 1:
- Toolchain selection and verification are paramount. Default configurations of even official/nightly builds can have limitations (e.g., multilib setup, implicit ISA extension enablement for the assembler).
- The `--print-multi-lib` GCC option is a valuable diagnostic for C library support.
- For some toolchains, ISA sub-extensions (like `_zicsr`, `_zifencei`) must be explicitly added to the `-march` string for the assembler to recognize all standard instructions, even if the base ISA version implies them by specification.
- xPack toolchains demonstrated robust multilib support.

Status for Task 1: COMPLETED.
