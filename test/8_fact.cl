// RUN: echo 10 > %t
// RUN: %ParaCLi %s < %t | FileCheck-10 %s --check-prefix=CHECK_INTERP

n = ?; // input
fact = 1;

while (n > 0)
{
  fact = fact * n;
  n = n - 1;
}

print fact; // CHECK_INTERP: 3628800

// RUN: rm %t
