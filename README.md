# Windows C/C++ Structured Exception Handling Validation Test Suite

Introduction
============

Windows Structured Exception Handling (SEH) is a C/C++ projection of the *Windows ABI Exception Dispatching and Reliable Stack Unwinding*. This projection is materialized as the following language keywords: ____try__, ____except__, ____finally__ and ____leave__.

In Windows, __setjmp/longjmp__ also implement Reliable Stack Unwinding when jumping to a another scope, higher in the stack.

SEH is implemented by 3 components:
-	Windows OS Runtime – As mentioned above, the OS provides the foundation services for Exception Dispatching and Reliable Stack Unwinding, on top of which the language-specific runtime provides its own syntax and contracts;
-	Compiler – the compiler is responsible for two very important roles:
    - Generate the necessary metadata or setup code required by the Windows OS Runtime to correctly perform EH Dispatching (looking for the handler) and Stack Unwinding. The amount and format of the information generated to achieve this varies by CPU architecture but, in most cases (e.g. x86_64, arm64 and arm), this involves adding metadata to the binaries that describes where each function saves important registers in the stack, and how much stack is used for local variables.
    - Generate the needed glue code, usually in the form of funclets, that allows the C-Runtime to invoke the code provided as part of the ____except__ filter and ____finally__ blocks. This might also include generating code to help the C-Runtime performing any local unwinding, such as restoring function-local significant register state.
-	C-Runtime – The C-language runtime provides the connection between the Windows OS Runtime and the code funclets that the compiler generated for the code around the SEH keywords. The C-Runtime will also be responsible for providing frame-local unwinding – in other words, handle the SEH flow control within the scope of one frame (function). Note that the C-Runtime also provides other exception syntaxes, like the standard C++ Exception Handling (__throw__ + __catch__), which do not stack on top of the Windows OS exception. In other words, C++ EH can handle its own language-specific exceptions, but not those raised by the OS or CPU (such as Access Violation or Division by Zero).


These tests provide comprehensive coverage of the scenarios a compiler and both runtimes may encounter when dealing with SEH. It is fundamental to pass these tests every time changes are made to the OS, the C-Runtime or the compiler.

Build the tests
===============
All following tests can and should be compiled and executed with varying degrees of optimizations, to make sure there are no issues or regressions specific to a given optimization or codegen option. However, they should always be compiled with an option to prevent function inlining. When using Microsoft Visual C++ compilers, that option is __/Ob0__. If inlining occurs, some of the tests may have reduced coverage. After compiling each test, just run the EXE. No parameters needed.


XCPT4
-----
This test focuses mostly on validating the compiler and the C-Runtime. It offers a complete coverage of local-frame SEH variations. This means, variations where the ____try__/____except__/____finally__ blocks involved, as well as the source of the exception are all within the same frame, for the greatest part. There are a few variations included where EH across multiple frames is also tested, but cross-frame and even cross-module is covered in a more comprehensive way in XFRAME_TEST below.

Note: It is OK to observe some skipped tests in the output. Not all variations are supported on all architectures. “passed” and “skipped” messages are benign. There should be no “failed” messages and the test should exit normally – no crashes.

NESTED_COLLIDED
---------------
As the name indicates, this test will validate the Nested Exception and Collided Unwind cases. In other words:
-	Exceptions that are raised within an __except filter (from a previous exception), which result in a Nested Dispatch;
-	Exceptions that are raised within a __finally block (from a previous exception), which result in a Collided Unwind.

Note: Test should complete successfully after printing two success messages, one for each case listed above.

XFRAME_TEST
-----------
This test was specifically designed to validate the correct EH across multiple modules – more specifically, the interoperability between binaries generated from different compilers. It involves two modules: one EXE and one DLL. To run this test, compile one of the modules (e.g. the EXE) with the latest stable Microsoft’s Visual C++ compiler, and the other module (e.g. the DLL) with the compiler you want to test. Then run the test. If it passes, switch which module gets compiled by which compiler and run the test again.

Note: Test should complete successfully after printing two success messages: one for handled exceptions and one for resumed exceptions.


# Contributing

This project welcomes contributions and suggestions.  Most contributions require you to agree to a
Contributor License Agreement (CLA) declaring that you have the right to, and actually do, grant us
the rights to use your contribution. For details, visit https://cla.microsoft.com.

When you submit a pull request, a CLA-bot will automatically determine whether you need to provide
a CLA and decorate the PR appropriately (e.g., label, comment). Simply follow the instructions
provided by the bot. You will only need to do this once across all repos using our CLA.

This project has adopted the [Microsoft Open Source Code of Conduct](https://opensource.microsoft.com/codeofconduct/).
For more information see the [Code of Conduct FAQ](https://opensource.microsoft.com/codeofconduct/faq/) or
contact [opencode@microsoft.com](mailto:opencode@microsoft.com) with any additional questions or comments.
