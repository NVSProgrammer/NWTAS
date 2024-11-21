// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "NWTAS.h"
#include "export.h"

using namespace std;
using namespace NWTAS;

void Err(string text){ throw runtime_error("Corupted DLL : "+text); }

bool TestRC() {
    if (removeComments(TestWV::RC) != TestCV::RC) {
        Err("Can NOT remove Comments");
        return false;
    }
    return true;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
            //if (!TestRC()) { ; return FALSE; }
            break;
        case DLL_THREAD_ATTACH:
            break;
        case DLL_THREAD_DETACH:
            break;
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}

