// RUN: %ParaCLi %s 2> %t || true
// RUN: FileCheck-10 %s --check-prefix=CHECK --input-file=%t

// RUN: %ParaCL %s 2> %t || true
// RUN: FileCheck-10 %s --check-prefix=CHECK --input-file=%t

x = 1 + + 2;  // обрабатывается корректно

// CHECK: expected: <MIN>  or  <NOT>  or  <LP>  or  <SCAN>  or  <INT>  or  <NAME> 
// CHECK-NEXT: before: <ADD>
