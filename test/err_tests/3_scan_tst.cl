// RUN: echo zzzzz > %t
// RUN: %ParaCLi %s < %t 2> %t || true
// RUN: FileCheck %s --check-prefix=CHECK --input-file=%t

x = ?;
print x;

// CHECK: Invalid symbol at stdin
