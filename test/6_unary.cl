// RUN: echo 10 > %t
// RUN: %ParaCLi %s < %t | FileCheck-10 %s --check-prefix=CHECK_INTERP

x = ?;

neg_x = -x; // -10
not_x = !x; // 0
not_not_x = !!x > 0; // 1
andrey = -!-x; // 0
min_min_x = --x; // 10

print 5 -- -x;   // CHECK_INTERP: -5
print neg_x;     // CHECK_INTERP-NEXT: -10 
print not_x;     // CHECK_INTERP-NEXT: 0
print not_not_x; // CHECK_INTERP-NEXT: 1
print andrey;    // CHECK_INTERP-NEXT: 0
print min_min_x; // CHECK_INTERP-NEXT: 10

// RUN: rm %t
