## How to use automatic testing?

* Install lit python packages via pip.
* Install FileCheck-10 util from llvm project.
* For testing run:
  ```bash
  /PATH_TO_TEST/my-lit.py /PATH_TO_TEST_OR_TEST_DIR
  ```

## How to write automatic test?

* Write ParaCL code which you want to test into `.cl` file.
* In comments write shell command to run. Write it after `RUN`. Like that:
  ```c
  // RUN: echo hello
  ```
* In comments write result you want to see and mark it with some tag (it is needed for FileCheck). For example:
  ```c
  // CHECK:      That what I want to see
  // CHECK-NEXT: in the output
  ```
* Check your command output with FileCheck. For example:
  ```c
  // RUN: echo hello | FileCheck-10 %s --check-prefix=MY_PREFIX
  ```
  More about FileCheck options in [documentation](https://llvm.org/docs/CommandGuide/FileCheck.html).

## More about lit tests:

* [lit documentation](https://llvm.org/docs/CommandGuide/lit.html)
* [FileCheck documentation](https://llvm.org/docs/CommandGuide/FileCheck.html)
* [helpful article](https://medium.com/@mshockwave/using-llvm-lit-out-of-tree-5cddada85a78)
* [another helpful article](https://pspdfkit.com/blog/2020/executable-api-documentation-and-llvm-integration-tester/)
