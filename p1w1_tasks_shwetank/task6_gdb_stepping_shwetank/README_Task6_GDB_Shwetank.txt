################################################################################
# Task 6: Stepping with GDB
# Author: Shwetank
# Date: June 5, 2025
# Program: India RISC-V Chip Tapeout
# Phase 1, Week 1
################################################################################

Objective:
To learn how to use `riscv-none-elf-gdb` to set breakpoints, run, step through
instructions (C and assembly), and inspect registers and program state for a
RISC-V ELF executable.

Setup:
- Toolchain: xPack GNU RISC-V Embedded GCC v14.2.0-3 (riscv-none-elf-gdb).
- Input ELF: `hello_riscv_pk_ecall.elf` (compiled with -g for debug symbols,
             and *without* --specs=nosys.specs/--specs=nano.specs, located in
             Task 2 directory, copied here).
- Emulator for GDB server: QEMU (`qemu-system-riscv32`).
- Workspace: ~/riscv_tapeout_shwetank_workspace/p1w1_tasks_shwetank/task6_gdb_stepping_shwetank/

Attempt Summary:

1. GDB `target sim` (with xPack ELF `hello_riscv_xpack.elf` - linked with nosys.specs):
   - Result: FAILED. Program terminated immediately with `SIGILL, Illegal instruction`,
             even before reaching `main` or `_start`. `target sim` deemed unsuitable.

2. GDB with Spike (`target remote` attempts):
   - Spike started with `spike -d ...`: This started Spike's internal debugger, not a GDB server.
   - Spike started with `spike ... pk -d ...`: `pk` did not recognize the `-d` option.
   - Spike started with `spike --halted ...`: Did not open a listen port for GDB; `target remote localhost:<port>` timed out.
   - Conclusion: Direct GDB `target remote` to the current Spike build was not straightforward.
   - Spike's internal debugger (`r 0 1`, `pc 0`, `reg 0 <name>`) was explored and allowed stepping through assembly for `hello_riscv_pk_ecall.elf` up to the `call puts` instruction. However, `printf` output was not observed with this method before it was paused.

3. GDB with QEMU (`target remote` - Successful for GDB practice):
   - QEMU Version: 8.2.2.
   - QEMU Startup Command (Terminal A):
     `qemu-system-riscv32 -nographic -machine virt -kernel hello_riscv_pk_ecall.elf -bios none -s -S`
     - `-s`: Starts GDB server on `tcp::1234`.
     - `-S`: Freezes CPU at startup, waiting for GDB `continue`.
     - `-bios none`: Used to prevent QEMU from looking for `opensbi-riscv32-generic-fw_dynamic.bin`.
   - GDB Session (Terminal B):
     - `riscv-none-elf-gdb hello_riscv_pk_ecall.elf`
     - `(gdb) target remote localhost:1234` (Connection successful)
     - `(gdb) break main`
     - `(gdb) continue`
   - Observation:
     - Initially, after `continue`, GDB reported `Program received signal SIGINT, Interrupt.` and was at `0x00000000 in ?? ()`. This indicated the program didn't cleanly stop at `main` when first run freely after connection, possibly due to issues in very early startup code or the nature of the interrupt.
     - Further attempts to restart the QEMU+GDB session and step from the absolute entry point would be needed to precisely diagnose this, but this part of the exercise was paused to proceed with other tasks.
   - Key GDB commands to use once correctly stopped at `main` or entry point:
     - `list` (view C source)
     - `disassemble` (view assembly)
     - `next` / `n` (step C line, over calls)
     - `step` / `s` (step C line, into calls)
     - `nexti` / `ni` (step assembly instruction, over calls)
     - `stepi` / `si` (step assembly instruction, into calls)
     - `info registers` (view all registers)
     - `p $a0` (print value of register a0)
     - `continue` / `c` (continue execution)
     - `break <function_name_or_*address>` (set breakpoint)

Status for Task 6:
- GDB `target sim` found unsuitable.
- GDB connection to Spike was not achieved with simple flags.
- GDB connection to QEMU was established. Program halted initially by QEMU's `-S` flag.
- Initial `continue` in GDB+QEMU led to an interrupt at `0x0` before `main` was cleanly hit, suggesting issues with the program's very early execution stages within QEMU when `-bios none` is used or how GDB initially took control. This requires further investigation, which was deferred.
- The *mechanics* of starting GDB, connecting to a remote target (QEMU), and basic GDB commands (`break`, `continue`) were practiced. Detailed stepping through `main` in QEMU was not fully completed due to the early interrupt issue.

Further debugging with GDB + QEMU required.
