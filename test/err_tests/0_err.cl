// RUN: %ParaCLi %s 2> %t || true
// RUN: FileCheck-10 %s --check-prefix=CHECK --input-file=%t

// RUN: %ParaCL %s 2> %t || true
// RUN: FileCheck-10 %s --check-prefix=CHECK --input-file=%t

x = 5;
y = 6
print x;

// CHECK:      syntax error in line: 9, column: 1
// CHECK-NEXT: expected: <SCOLON>  or  <OR>  or  <AND>  or  <IS_EQ>  or  <NOT_EQ>  or  <GREATER>  or  <LESS>  or  <GR_EQ>  or  <LS_EQ>  or  <ADD>  or  <MIN>  or  <MUL>  or  <DIV>  or  <MOD> 
// CHECK-NEXT: before: <PRINT>
// CHECK-NEXT: 9   |   print x;
// CHECK-NEXT:     |   ^~~~~
