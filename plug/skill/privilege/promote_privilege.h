#ifndef _PROMOTE_PRIVILEGE_H_
#define _PROMOTE_PRIVILEGE_H_

#include <string>

#include <Windows.h>

namespace plug
{
HANDLE OpenProcess(DWORD processId);

BOOL PromotePrivilege();

BOOL ProcessSnapshoot(const std::wstring& processName, int* processId);

BYTE* ProcessSnapshootModule(const std::wstring&dllName, const int processID);

BOOL SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivileg);

}

#endif