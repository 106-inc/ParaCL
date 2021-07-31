// RUN: %ParaCLi %s | FileCheck %s --check-prefix=CHECK

a = 1;
b = 2;

// this is comment
print a+b; // CHECK: 3

