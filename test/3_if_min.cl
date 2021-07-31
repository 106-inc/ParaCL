// RUN: %ParaCLi %s | FileCheck %s --check-prefix=CHECK

if (0)
  print 228 + 1447; // CHECK-NOT: 1675
else if (1)
{
  if (0)
    print 1488; // CHECK-NOT: 1488
  else if (1)
  {
    print 1; // CHECK: 1
  }
}
else 
  print 0; // CHECK-NOT: 0
