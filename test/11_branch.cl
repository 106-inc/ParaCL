// RUN: %ParaCLi %s | FileCheck-10 %s --check-prefix=CHECK_INTERP

if (0)
  print 1; // CHECK_INTERP-NOT: 1
else if (0)
  print 2; // CHECK_INTERP-NOT: 2
else if (1)
  print 3; // CHECK_INTERP: 3

if (1) { print 4; } // CHECK_INTERP: 4

res = 0;
while (res < 5) { res = res + 1; }

if (res >= 5) { print res; } // CHECK_INTERP: 5

if (0)
  print 6; // CHECK_INTERP-NOT: 6
else if (1) {
  print 7; // CHECK_INTERP: 7
}