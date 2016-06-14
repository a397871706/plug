// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"

HMODULE gModule = NULL;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
    case DLL_PROCESS_ATTACH:
    {
        gModule = hModule;
        ::DisableThreadLibraryCalls(hModule);
        break;
    }
	case DLL_THREAD_ATTACH:
        break;
	case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
    {
        gModule = NULL;
        break;
    }
	}
	return TRUE;
}

