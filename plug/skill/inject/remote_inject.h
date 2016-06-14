#ifndef _REMOTE_INJECT_H_
#define _REMOTE_INJECT_H_

#include <Windows.h>
#include <winternl.h>
#include <string>
#include <memory>

#include <base/win/scoped_handle.h>

class HookInterface;

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
    void Init();
    void Release();

private:
    typedef void (WINAPI *LoadLibraryAddres)(wchar_t*);
    typedef DWORD(WINAPI *NtCreateThreadEx)(PHANDLE, ACCESS_MASK,
                                            POBJECT_ATTRIBUTES, HANDLE,
                                            LPTHREAD_START_ROUTINE, LPVOID,
                                            BOOL, DWORD, DWORD, DWORD, LPVOID);
    typedef HookInterface* (__stdcall *CreateHookInterface)();
    typedef void(__stdcall * RelaseHookInterface)(HookInterface*);

    bool InjectDllByRemoteThread(const std::wstring& procName);
    bool InjectDllByHook();
    bool InjectDllByRegedit();

    void OnNtCreateThread(const LoadLibraryAddres& proc,
                          const NtCreateThreadEx& NtCreateThreadObject,
                          const base::win::ScopedHandle& handle, void* param);

    HookInterface* hook_;
    std::shared_ptr<void> dll_;
    RelaseHookInterface releaseHook_;
};

#endif