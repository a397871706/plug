#ifndef _PROMOTE_PRIVILEGE_H_
#define _PROMOTE_PRIVILEGE_H_

#include <string>

#include <Windows.h>

namespace plug
{
HANDLE OpenProcess(DWORD processId);

BOOL PromotePrivilege();

BOOL ProcessSnapshoot(const std::wstring& processName, int processId);

BOOL SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivileg);

}

#endif