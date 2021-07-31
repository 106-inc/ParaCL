// RUN: echo "1 8" > %t
// RUN: %ParaCLi %s < %t | FileCheck %s --check-prefix=CHECK

n = ? + 1 + ?*2;

print n; // CHECK: 18

// RUN: rm %t
