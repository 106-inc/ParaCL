// RUN: %ParaCLi %s | FileCheck-10 %s --check-prefix=CHECK_INTERP

putc 42; // CHECK_INTERP: *
