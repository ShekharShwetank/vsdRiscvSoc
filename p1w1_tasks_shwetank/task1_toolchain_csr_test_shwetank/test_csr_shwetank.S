// File: test_csr_shwetank.S
// Purpose: Basic test for CSR instruction assembly.
.globl _start
_start:
  csrr t0, mhartid
  csrr t1, misa
loop:
  j loop
