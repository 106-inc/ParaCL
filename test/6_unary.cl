// RUN: echo 10 > %t
// RUN: %ParaCLi %s < %t | FileCheck %s --check-prefix=CHECK

x = ?;

neg_x = -x; // -10
not_x = !x; // 0
not_not_x = !!x > 0; // 1
andrey = -!-x; // 0
min_min_x = --x; // 10

print 5 -- -x;   // CHECK: -5
print neg_x;     // CHECK-NEXT: -10 
print not_x;     // CHECK-NEXT: 0
print not_not_x; // CHECK-NEXT: 1
print andrey;    // CHECK-NEXT: 0
print min_min_x; // CHECK-NEXT: 10

// RUN: rm %t
