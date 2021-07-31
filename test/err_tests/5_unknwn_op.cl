// RUN: %ParaCLi %s 2> %t || true
// RUN: FileCheck %s --check-prefix=CHECK --input-file=%t

// RUN: %ParaCL %s 2> %t || true
// RUN: FileCheck %s --check-prefix=CHECK --input-file=%t

t = 4 ' 0;

// CHECK:      syntax error in line: 7, column: 7
// CHECK-NEXT: expected: <ADD>  or  <MIN>  or  <MUL>  or  <DIV>  or  <MOD>  or  <GREATER>  or  <LESS>  or  <GR_EQ>  or  <LS_EQ>  or  <IS_EQ>  or  <NOT_EQ>  or  <AND>  or  <OR>  or  <SCOLON> 
// CHECK-NEXT: before: <ERR>
// CHECK-NEXT: 7   |   t = 4 ' 0;
// CHECK-NEXT:     |   ~~~~~~^