// RUN: %ParaCLi %s | FileCheck %s --check-prefix=CHECK

print 5 > 10; // CHECK: 0
print 5 >= 5; // CHECK-NEXT: 1
print 1 <= 1; // CHECK-NEXT: 1
print 1 < 1; // CHECK-NEXT: 0

print 1 || 0; // CHECK-NEXT: 1
print 1 && 0; // CHECK-NEXT: 0
