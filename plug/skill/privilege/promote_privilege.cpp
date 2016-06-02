#include "promote_privilege.h"

#include <tchar.h>
#include <Tlhelp32.h>

#include <base/win/scoped_handle.h>

using base::win::ScopedHandle;

namespace plug
{

HANDLE OpenProcess(DWORD processId)
{
    HANDLE process = ::OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE |
                                   PROCESS_VM_OPERATION |
                                   PROCESS_QUERY_INFORMATION |
                                   PROCESS_CREATE_THREAD, FALSE, processId);
    return process;
}

BOOL PromotePrivilege()
{
    HANDLE token = NULL;
    ::OpenProcessToken(::GetCurrentProcess(), TOKEN_QUERY |
                       TOKEN_ADJUST_PRIVILEGES, &token);
    if (token)
    {
        if (SetPrivilege(token, SE_DEBUG_NAME, TRUE))
        {
            ::CloseHandle(token);
            return TRUE;
        }
    }

    return FALSE;
}

BOOL ProcessSnapshoot(const std::wstring& processName, int* procrssId)
{
    if (!procrssId)
        return FALSE;

    HANDLE snapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (snapshot == INVALID_HANDLE_VALUE)   
    {
        ::CloseHandle(snapshot);
        return FALSE;
    }
    PROCESSENTRY32 pr32 = { 0 };
    pr32.dwSize = sizeof(pr32);
    pr32.cntUsage = 0;
    ::Process32First(snapshot, &pr32);
    do 
    {
        if (_tcscmp(pr32.szExeFile, processName.c_str()) == 0)
        {
            *procrssId = pr32.th32ProcessID;
            break;
        }
    } while (::Process32Next(snapshot, &pr32));

    ::CloseHandle(snapshot);
    return TRUE;
}

BOOL SetPrivilege(
    HANDLE hToken,          // access token handle
    LPCTSTR lpszPrivilege,  // name of privilege to enable/disable
    BOOL bEnablePrivilege   // to enable or disable privilege
    )
{
    TOKEN_PRIVILEGES tp;
    LUID luid;

    if (!LookupPrivilegeValue(
        NULL,            // lookup privilege on local system
        lpszPrivilege,   // privilege to lookup 
        &luid))        // receives LUID of privilege
    {
        return FALSE;
    }

    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    if (bEnablePrivilege)
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    else
        tp.Privileges[0].Attributes = 0;

    // Enable the privilege or disable all privileges.

    if (!AdjustTokenPrivileges(
        hToken,
        FALSE,
        &tp,
        sizeof(TOKEN_PRIVILEGES),
        (PTOKEN_PRIVILEGES)NULL,
        (PDWORD)NULL))
    {
        return FALSE;
    }

    if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)

    {
        return FALSE;
    }

    return TRUE;
}

BYTE* ProcessSnapshootModule(const std::wstring&dllName, const int processID)
{
    MODULEENTRY32 me = {};
    me.dwSize = sizeof(me);
    ScopedHandle handle(::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processID));
    if (handle.IsValid())
    {
        ::Module32First(handle.Get(), &me);
        for (;;::Module32Next(handle.Get(), &me))
        {
            if (!_tcsicmp(reinterpret_cast<wchar_t*>(me.szModule), dllName.c_str()) ||
                !_tcsicmp(reinterpret_cast<wchar_t*>(me.szExePath), dllName.c_str()))
            {
                return me.modBaseAddr;
            }
        }
    }

    return nullptr;
}

}

