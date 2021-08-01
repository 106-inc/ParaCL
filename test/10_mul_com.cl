// RUN: %ParaCLi %s | FileCheck-10 %s --check-prefix=CHECK_INTERP

r = 5;
/* trying */
print r; // CHECK_INTERP: 5
