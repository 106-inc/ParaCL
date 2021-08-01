// RUN: %ParaCLi %s 2> %t || true
// RUN: FileCheck-10 %s --check-prefix=CHECK --input-file=%t

// RUN: %ParaCL %s 2> %t || true
// RUN: FileCheck-10 %s --check-prefix=CHECK --input-file=%t

x = 1 + + 2;  // обрабатывается корректно

// CHECK: expected: <LP>  or  <SCAN>  or  <INT>  or  <NAME>  or  <MIN>  or  <NOT> 
// CHECK-NEXT: before: <ADD>
