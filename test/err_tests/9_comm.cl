// RUN: %ParaCLi %s 2> %t || true
// RUN: FileCheck %s --check-prefix=CHECK --input-file=%t

// RUN: %ParaCL %s 2> %t || true
// RUN: FileCheck %s --check-prefix=CHECK --input-file=%t

a = 5;
c = 6 + /**//**/
/*helle a = fksdljfkldsjgsgkldkf;ds ;slf l;gm; ldsg
  glfdjgklsdfglkd
  gflsdfkglk
  dl;gk
  dsgks;lgk
  sdkg
  kas
  gka;s 
  print ngdkmgds */ 7 ;
b = 5/*;
print a */+  + c;
print b;


// CHECK:      syntax error in line: 19, column: 14
// CHECK-NEXT: expected: <MIN>  or  <NOT>  or  <LP>  or  <SCAN>  or  <INT>  or  <NAME> 
// CHECK-NEXT: before: <ADD>
// CHECK-NEXT: 19   |   print a */+  + c;
// CHECK-NEXT:      |   ~~~~~~~~~~~~~^
