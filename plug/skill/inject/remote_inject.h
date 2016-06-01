#ifndef _REMOTE_INJECT_H_
#define _REMOTE_INJECT_H_

#include <Windows.h>
#include <winternl.h>
#include <string>

#include <base/win/scoped_handle.h>

class RemoteInject
{
public:
    enum InjectType
    {
        REMOTE_THREAD,
        HOOK,
        REGEDIT,
    };

    RemoteInject();
    ~RemoteInject();

    bool InjectDll(InjectType type);

private:
    typedef void (WINAPI *LoadLibraryAddres)(wchar_t*);
    typedef DWORD(WINAPI *NtCreateThreadEx)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES, HANDLE, LPTHREAD_START_ROUTINE, LPVOID, BOOL, DWORD, DWORD, DWORD, LPVOID);

    bool InjectDllByRemoteThread(const std::wstring& procName);
    bool InjectDllByHook();
    bool InjectDllByRegedit();

    void OnNtCreateThread(const LoadLibraryAddres& proc,
                          const NtCreateThreadEx& NtCreateThreadObject,
                          const base::win::ScopedHandle& handle, void* param);
};

#endif