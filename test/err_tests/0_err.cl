// RUN: %ParaCLi %s 2> %t || true
// RUN: FileCheck-10 %s --check-prefix=CHECK --input-file=%t

// RUN: %ParaCL %s 2> %t || true
// RUN: FileCheck-10 %s --check-prefix=CHECK --input-file=%t

x = 5;
y = 6
print x;

// CHECK: expected: <ADD>  or  <MIN>  or  <MUL>  or  <DIV>  or  <MOD>  or  <GREATER>  or  <LESS>  or  <GR_EQ>  or  <LS_EQ>  or  <IS_EQ>  or  <NOT_EQ>  or  <AND>  or  <OR>  or  <SCOLON> 
// CHECK-NEXT: before: <PRINT>
