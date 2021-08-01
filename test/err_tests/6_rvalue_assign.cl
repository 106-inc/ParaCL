// RUN: %ParaCLi %s 2> %t || true
// RUN: FileCheck-10 %s --check-prefix=CHECK --input-file=%t

// RUN: %ParaCL %s 2> %t || true
// RUN: FileCheck-10 %s --check-prefix=CHECK --input-file=%t

x = 3;
5 = x;

// CHECK:      syntax error in line: 8, column: 1
// CHECK-NEXT: expected: <end of file>  or  <LB>  or  <IF>  or  <WHILE>  or  <PRINT>  or  <NAME> 
// CHECK-NEXT: before: <INT>
// CHECK-NEXT: 8   |   5 = x;
// CHECK-NEXT:     |   ^
