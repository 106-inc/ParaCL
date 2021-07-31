// RUN: %ParaCLi %s 2> %t || true
// RUN: FileCheck %s --check-prefix=CHECK --input-file=%t

a = 5 / 0;
print a;

//CHECK: Dividing by ZERO!
