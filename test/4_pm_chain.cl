// RUN: %ParaCLi %s | FileCheck %s --check-prefix=CHECK_INTERP

a = 5;
b = 5 - 6 + a;
print b; // CHECK_INTERP: 4
