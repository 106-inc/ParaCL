// RUN: %ParaCLi %s | %FileCheck %s --check-prefix=CHECK

a = 5;
b = 5 - 6 + a;
print b; // CHECK: 4
