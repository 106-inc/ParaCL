// RUN: echo "1 8" > %t
// RUN: %ParaCLi %s < %t | %FileCheck %s --check-prefix=CHECK_INTERP

n = ? + 1 + ?*2;

print n; // CHECK_INTERP: 18

// RUN: rm %t
