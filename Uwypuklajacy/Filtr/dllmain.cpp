﻿// dllmain.cpp : Definiuje punkt wejścia dla aplikacji DLL.
#include "pch.h"

//BIBLIOTEKA Z FUNKCJAMI C++

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}


#define EOF (-1)

#ifdef __cplusplus    // If used by C++ code, 
extern "C" {          // we need to export the C interface
#endif

    __declspec(dllexport) int div(int a, int b)
    {
        return a / b;
    }

#ifdef __cplusplus
}
#endif