// RUN: echo 10 > %t
// RUN: %ParaCLi %s < %t | FileCheck %s --check-prefix=CHECK

n = ?; // input
fact = 1;

while (n > 0)
{
  fact = fact * n;
  n = n - 1;
}

print fact; // CHECK: 3628800

// RUN: rm %t
