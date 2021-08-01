// RUN: %ParaCLi %s 2> %t || true
// RUN: FileCheck-10 %s --check-prefix=CHECK --input-file=%t

// RUN: %ParaCL %s 2> %t || true
// RUN: FileCheck-10 %s --check-prefix=CHECK --input-file=%t

x = 1 // обрабатывается корректно

// CHECK: syntax error in line: 12, column: 240
// CHECK-NEXT: expected: <SCOLON>  or  <OR>  or  <AND>  or  <IS_EQ>  or  <NOT_EQ>  or  <GREATER>  or  <LESS>  or  <GR_EQ>  or  <LS_EQ>  or  <ADD>  or  <MIN>  or  <MUL>  or  <DIV>  or  <MOD> 
// CHECK-NEXT: before: <end of file>
