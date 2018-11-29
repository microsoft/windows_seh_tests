/*++

Copyright (c) Microsoft Corporation

Module Name:

    xcpt4cxx.cpp

Abstract:

    This module implements user mode exception tests for C++ interaction.

--*/

#include "stdio.h"
#include "setjmpex.h"
#include "float.h"
#include "windows.h"
#include "math.h"


DECLSPEC_NOINLINE
VOID
Test82Foo (
    VOID
    )

{

    throw 1;
}

extern "C"
VOID
Test82 (
    _Inout_ PLONG Counter
    )

{

    int retval = 1;
  
    __try {
       __try {
           Test82Foo();

       } __finally {
            switch(*Counter) {
            case 0:
               printf("something failed!\n");
               retval = 6;
               break;

            case 1:
               retval = 0;
               break;

            case 2:
               printf("how did you get here?\n");
               retval = 2;
               break;

            case 3:
               printf("what?!?\n");
               retval = 3;
               break;

            case 4:
               printf("not correct\n");
               retval = 4;
               break;

            case 5:
               printf("error!\n");
               retval = 5;
               break;
            }
        }

    } __except(1) {
    }

    *Counter = retval;
    return;
}

template<int Instance>
class Destructable
{

public:

    Destructable(int n) : id(n)
    {
        alive += 1;
        return;
    }

    ~Destructable()
    {
        alive -= 1;
        return;
    }

    int id;
    static int alive;
};

template<int Instance> int Destructable<Instance>::alive = 0;

VOID
CxxLongjump90 (
    VOID
    )

{
    jmp_buf env;
    int status;
    Destructable<90> l0(0);

    status = setjmp(env);

    if (status == 0) {
        Destructable<90> l1(1);

        longjmp(env, 1);
    }
    else {
        Destructable<90> l2(2);
    }

    return;
}

extern "C"
VOID
Test90 (
    _Inout_ PLONG State
    )

{
    CxxLongjump90();

    *State = Destructable<90>::alive;
    return;
}
