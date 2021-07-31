// RUN: echo 20 > %t
// RUN: %ParaCLi %s < %t | FileCheck %s --check-prefix=CHECK_INTERP

n = ?; //input fib number

i = 1;
fib = 1;
fib_prev = 0;

if (n <= 0)
  fib = 0;
else
  while (!(i == n))
  {
    old_fib = fib;
    fib = fib + fib_prev;
    fib_prev = old_fib;

    i = i + 1;
  }

print fib; // CHECK_INTERP: 6765

// RUN: rm %t
