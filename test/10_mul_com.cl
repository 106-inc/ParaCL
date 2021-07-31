// RUN: %ParaCLi %s | %FileCheck %s --check-prefix=CHECK

r = 5;
/* trying */
print r; // CHECK: 5
