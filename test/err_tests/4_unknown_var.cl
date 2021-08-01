// RUN: %ParaCLi %s 2> %t || true
// RUN: FileCheck-10 %s --check-prefix=CHECK --input-file=%t

// RUN: %ParaCL %s 2> %t || true
// RUN: FileCheck-10 %s --check-prefix=CHECK --input-file=%t

a = 1;
x = a + b;

// CHECK:      Unknown variable 'b'
// CHECK-NEXT: syntax error in line: 8, column: 9
// CHECK-NEXT: before: <UNKNOWN_VAR>
// CHECK-NEXT: 8   |   x = a + b;
// CHECK-NEXT:     |   ~~~~~~~~^
