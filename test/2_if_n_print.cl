// RUN: %ParaCLi %s | FileCheck-10 %s --check-prefix=CHECK_INTERP

d = 1;
x = 4 + d;
if (x)
    print x; // CHECK_INTERP: 5
