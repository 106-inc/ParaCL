// RUN: %ParaCLi %s | %FileCheck %s --check-prefix=CHECK_INTERP

print 5 > 10; // CHECK_INTERP: 0
print 5 >= 5; // CHECK_INTERP-NEXT: 1
print 1 <= 1; // CHECK_INTERP-NEXT: 1
print 1 < 1; // CHECK_INTERP-NEXT: 0

print 1 || 0; // CHECK_INTERP-NEXT: 1
print 1 && 0; // CHECK_INTERP-NEXT: 0
