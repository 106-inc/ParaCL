// RUN: %ParaCLi %s | FileCheck %s --check-prefix=CHECK

d = 1;
x = 4 + d;
if (x)
    print x; // CHECK: 5
