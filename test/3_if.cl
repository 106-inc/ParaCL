// RUN: %ParaCLi %s | FileCheck-10 %s --check-prefix=CHECK_INTERP

if (0)
  print 228 + 1447; // CHECK_INTERP-NOT: 1675
else if (1)
{
  if (0)
    print 1488; // CHECK_INTERP-NOT: 1488
  else if (1)
  {
    if (0)
      print 1; // CHECK_INTERP-NOT: 1
    if (1)
      print 1337; // CHECK_INTERP: 1337
  }
}
else 
  print 0; // CHECK_INTERP-NOT: 0
