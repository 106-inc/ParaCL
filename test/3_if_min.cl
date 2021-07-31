// RUN: %ParaCLi %s | FileCheck %s --check-prefix=CHECK_INTERP

if (0)
  print 228 + 1447; // CHECK_INTERP-NOT: 1675
else if (1)
{
  if (0)
    print 1488; // CHECK_INTERP-NOT: 1488
  else if (1)
  {
    print 1; // CHECK_INTERP: 1
  }
}
else 
  print 0; // CHECK_INTERP-NOT: 0
