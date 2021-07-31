// RUN: %ParaCLi %s | %FileCheck %s --check-prefix=CHECK_INTERP

a = 1;
b = 2;

// this is comment
print a+b; // CHECK_INTERP: 3

