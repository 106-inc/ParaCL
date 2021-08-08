// RUN: %ParaCLi %s | FileCheck-10 %s --check-prefix=CHECK_INTERP

putc 72;
putc 101;
putc 108;
putc 108;
putc 111;

putc 44;
putc 32;

putc 119;
putc 111;
putc 114;
putc 108;
putc 100;

putc 33;

putc 10;

// CHECK_INTERP: Hello, world!
