################################################################################
# Task 7: Running Under an Emulator
# Author: Shwetank
# Date: June 5, 2025
# Program: India RISC-V Chip Tapeout
# Phase 1, Week 1
################################################################################

Objective:
To run the "Hello, RISC-V!" bare-metal ELF executable under an emulator (Spike or QEMU) and observe printf output on the host console.

Summary of Efforts:

Part 1: Spike + Proxy Kernel (pk) - SUCCESSFUL

1. Spike Installation:
   - Source: `riscv-isa-sim` from `https://github.com/riscv-software-src/riscv-isa-sim.git`.
   - Build: Configured with `../configure --prefix=$HOME/riscv_custom_tools`, then `make -j$(nproc)` and `make install`.
   - Verification: `spike --help` confirmed version 1.1.1-dev. PATH updated.

2. Proxy Kernel (pk) Build:
   - Source: `riscv-pk` from `https://github.com/riscv-software-src/riscv-pk.git`.
   - Toolchain: xPack GNU RISC-V Embedded GCC v14.2.0-3 (`riscv-none-elf-gcc`).
   - Critical Configure Command in `riscv-pk/build-rv32imac-xpack/` directory:
     `../configure --prefix=$HOME/riscv_custom_tools --host=riscv-none-elf --with-arch=rv32imac_zicsr_zifencei --with-abi=ilp32 CC=riscv-none-elf-gcc`
     (Note: Explicitly adding `_zicsr_zifencei` to `--with-arch` was essential to allow `pk` to compile, as the xPack assembler needed these for CSR and FENCE.I instructions used in `pk`'s source code).
   - Build: `make -j$(nproc)` (Successful).
   - Install: `make install` (Installed `pk` to `$HOME/riscv_custom_tools/riscv-none-elf/bin/pk`). PATH updated.

3. C Application (`hello_riscv_pk_ecall.elf`):
   - Source: `hello_riscv_shwetank.c` (from Task 2).
   - Crucial Compilation Command (using xPack GCC, *without* nosys.specs/nano.specs to enable proper syscalls for pk):
     `cd ~/riscv_tapeout_shwetank_workspace/p1w1_tasks_shwetank/task2_hello_riscv_shwetank/`
     `riscv-none-elf-gcc -march=rv32imac -mabi=ilp32 -g -o hello_riscv_pk_ecall.elf hello_riscv_shwetank.c`

4. Execution with Spike + pk:
   - Navigated to `~/riscv_tapeout_shwetank_workspace/p1w1_tasks_shwetank/task7_emulator_shwetank/`.
   - Copied `hello_riscv_pk_ecall.elf` to this directory.
   - Command:
     `spike --isa=rv32imac /home/ank/riscv_custom_tools/riscv-none-elf/bin/pk ./hello_riscv_pk_ecall.elf`
   - **Observed Output: `Hello, RISC-V! Shwetank is here.`**

Part 2: QEMU Attempts (for GDB connection in Task 6, and potential for Task 7 output)

1. QEMU Version: 8.2.2.
2. Running `hello_riscv_pk_ecall.elf` with QEMU for GDB:
   - Command (Terminal A):
     `qemu-system-riscv32 -nographic -machine virt -kernel hello_riscv_pk_ecall.elf -bios none -s -S`
   - This started QEMU and waited for GDB on port 1234. The `-bios none` was necessary to prevent errors about missing `opensbi-riscv32-generic-fw_dynamic.bin`.
   - GDB Connection (Terminal B): `target remote localhost:1234` (Successful).
3. `printf` Output with QEMU using above command:
   - Not observed. This is expected because running with `-kernel ... -bios none` means there's no environment (like OpenSBI or pk *running inside QEMU*) to handle the `ecall` instructions that `newlib`'s `printf` would generate.
   - To get `printf` in QEMU, further steps like using OpenSBI or implementing custom `_write` syscalls for QEMU's UART would be needed (related to Task 16).

Key Learnings from Task 7:
- Successfully ran a C program using `printf` on the Spike emulator via the Proxy Kernel (`pk`).
- Discovered the importance of compiling the application for `pk` *without* `nosys.specs` so that `newlib` generates actual `ecall` instructions.
- Found that `pk` itself required a very specific `-march` string (`rv32imac_zicsr_zifencei`) to build correctly with the chosen xPack toolchain due to assembler requirements for CSR and FENCE.I instructions.
- Basic QEMU execution with `-kernel` and `-bios none` works for running an ELF, but `printf` output requires additional setup (like OpenSBI or custom syscall implementations for QEMU's UART).

Status for Task 7:
- Spike + pk portion: COMPLETED and `printf` output achieved.
- QEMU portion: Basic execution achieved; `printf` output not yet achieved (would require further steps like OpenSBI or custom syscalls). For now, the Spike success meets the core requirement.
