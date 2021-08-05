// RUN: %ParaCLi %s | FileCheck-10 %s --check-prefix=CHECK_INTERP

print 1 + {res = 1 + 1;}; // CHECK_INTERP: 3
a = 228;
print {
  if (1)
    a = a + 2;
}; // CHECK_INTERP: 230
