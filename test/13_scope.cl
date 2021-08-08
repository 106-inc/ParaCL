// RUN: %ParaCLi %s | FileCheck-10 %s --check-prefix=CHECK_INTERP

x = {1 + 2;} + 3;
print x; // CHECK_INTERP: 6
