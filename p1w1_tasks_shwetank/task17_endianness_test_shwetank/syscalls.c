/*
 * File: syscalls.c
 * Author: Shwetank
 * Date: June 7, 2025
 * Task: p1w1 - Task 16: Retargeting printf
 * Purpose: Provides a complete bare-metal syscall shim for Newlib.
 */

#include <stdint.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

#undef errno
extern int errno;

// The memory-mapped address of the QEMU virt machine's UART
#define UART_TX_ADDRESS 0x10000000
volatile char *uart_tx = (volatile char *)UART_TX_ADDRESS;


// --- Minimal implementation of required syscalls ---

int _write(int file, char *ptr, int len) {
    (void)file; // Unused parameter
    for (int i = 0; i < len; i++) {
        *uart_tx = ptr[i];
    }
    return len;
}

int _close(int file) {
    (void)file;
    return -1;
}

int _fstat(int file, struct stat *st) {
    (void)file;
    st->st_mode = S_IFCHR;
    return 0;
}

int _isatty(int file) {
    (void)file;
    return 1;
}

int _lseek(int file, int ptr, int dir) {
    (void)file;
    (void)ptr;
    (void)dir;
    return 0;
}

int _read(int file, char *ptr, int len) {
    (void)file;
    (void)ptr;
    (void)len;
    return 0;
}

// A very simple 'brk'/'sbrk' implementation for heap management
// The linker script must provide the _end symbol.
extern char _end[];
static char *heap_end = _end;

void *_sbrk(intptr_t incr) {
    char *prev_heap_end = heap_end;
    heap_end += incr;
    return (void *)prev_heap_end;
}
