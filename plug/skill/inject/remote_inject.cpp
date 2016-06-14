#include "remote_inject.h"

#include <base/basictypes.h>
#include <base/files/file_path.h>

#include "../privilege/promote_privilege.h"
#include "../../quirky/hook_test.h"

using base::win::ScopedHandle;

namespace
{
const wchar_t* InjectDllName = L"quirky.dll";
}

RemoteInject::RemoteInject()
    : hook_(nullptr)
    , dll_()
    , releaseHook_()
{

}

RemoteInject::~RemoteInject()
{

}

bool RemoteInject::InjectDll(InjectType type)
{
 //   InjectDllByRemoteThread(L"MFCApplication1.exe");
    InjectDllByHook();
    return true;
}

bool RemoteInject::InjectDllByRemoteThread(const std::wstring& procName)
{
    int processID = 0;
    if (!plug::ProcessSnapshoot(procName, &processID) && (processID == 0))
        return false;

    ScopedHandle tagerProcess(plug::OpenProcess(processID));
    if (!tagerProcess.IsValid())
    {
        if (plug::PromotePrivilege())
        {
            tagerProcess.Set(plug::OpenProcess(processID));
            if (!tagerProcess.IsValid())
                return false;
        }
    }

    wchar_t dllpath[MAX_PATH] = { 0 };
    ::GetModuleFileName(NULL, dllpath, MAX_PATH);
    base::FilePath filepath(dllpath);
    base::FilePath p = filepath.DirName();
    p = p.Append(InjectDllName);
    uint32 strBuffer = (p.value().length() + 1) * sizeof(wchar_t);
    void* strBufPoint = ::VirtualAllocEx(tagerProcess.Get(), NULL, strBuffer,
                                         MEM_COMMIT | MEM_RESERVE,
                                         PAGE_READWRITE);
    if (!strBufPoint)
    {
        return false;
    }


    SIZE_T out = 0;
    wcscpy_s(dllpath, strBuffer, p.value().c_str());
    if (!::WriteProcessMemory(tagerProcess.Get(), strBufPoint,
                              reinterpret_cast<void*>(&dllpath[0]), strBuffer,
                              &out))
    {
        ::VirtualFreeEx(tagerProcess.Get(), strBufPoint, strBuffer, MEM_DECOMMIT);
        return false;
    }
    
    LoadLibraryAddres LoadLibraryObject =
        reinterpret_cast<LoadLibraryAddres>(
            ::GetProcAddress(::GetModuleHandle(L"Kernel32.dll"), "LoadLibraryW"));

    NtCreateThreadEx NtCreateThreadObject =
        reinterpret_cast<NtCreateThreadEx>(
        ::GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "NtCreateThreadEx"));
    if (NtCreateThreadObject)
    {
        OnNtCreateThread(LoadLibraryObject, NtCreateThreadObject, tagerProcess,
                         strBufPoint);
    }
    else
    {
        ScopedHandle remote(::CreateRemoteThread(
            tagerProcess.Get(), NULL, NULL,
            reinterpret_cast<LPTHREAD_START_ROUTINE>(LoadLibraryObject),
            strBufPoint, NULL, NULL));
        DWORD err = ::GetLastError();
        if (!remote.IsValid())
        {
            ::VirtualFreeEx(tagerProcess.Get(), strBufPoint, strBuffer,
                            MEM_DECOMMIT);
            return false;
        }

        ::WaitForSingleObject(remote.Get(), INFINITE);
    }

    ::VirtualFreeEx(tagerProcess.Get(), strBufPoint, strBuffer, MEM_DECOMMIT);
    return true;
}

bool RemoteInject::InjectDllByHook()
{
    if (hook_)
        hook_->SetHook(0);

    return true;
}

bool RemoteInject::InjectDllByRegedit()
{
    return true;
}

void RemoteInject::OnNtCreateThread(const LoadLibraryAddres& proc,
                                    const NtCreateThreadEx& NtCreateThreadObject,
                                    const base::win::ScopedHandle& handle,
                                    void* param)
{
    plug::PromotePrivilege();
    struct UNKNOWN
    {
        ULONG Length;
        ULONG Unknown1;
        ULONG Unknown2;
        PULONG Unknown3;
        ULONG Unknown4;
        ULONG Unknown5;
        ULONG Unknown6;
        PULONG Unknown7;
        ULONG Unknown8;
    };

    UNKNOWN buffer = { 0 };
    memset(&buffer, 0, sizeof(buffer));
    DWORD dw0 = 0;
    DWORD dw1 = 0;

    buffer.Length = sizeof(buffer);
    buffer.Unknown1 = 0x10003;
    buffer.Unknown2 = 0x8;
    buffer.Unknown3 = &dw1;
    buffer.Unknown4 = 0;
    buffer.Unknown5 = 0x10004;
    buffer.Unknown6 = 4;
    buffer.Unknown7 = &dw0;
    buffer.Unknown8 = 0;

    if (NtCreateThreadObject)
    {
        HANDLE remote = NULL;
        NtCreateThreadObject(&remote, 0x1FFFFF, NULL, handle.Get(),
                             reinterpret_cast<LPTHREAD_START_ROUTINE>(proc),
                             param, FALSE, NULL, NULL, NULL, NULL);
        ::WaitForSingleObject(remote, INFINITE);
        ::CloseHandle(remote);
    }
}

void RemoteInject::Init()
{
    std::shared_ptr<void> dll(::LoadLibrary(InjectDllName), ::FreeLibrary);
    if (dll)
    {
        dll_ = dll;

        CreateHookInterface createHook = reinterpret_cast<CreateHookInterface>(
            ::GetProcAddress(reinterpret_cast<HMODULE>(dll_.get()), "SetHook"));

        DWORD err = ::GetLastError();

        RelaseHookInterface releaseHook = reinterpret_cast<RelaseHookInterface>(
            ::GetProcAddress(reinterpret_cast<HMODULE>(dll_.get()), "Release"));

        if (createHook)
            hook_ = createHook();

        if (releaseHook)
            releaseHook_ = releaseHook;
    }
}

void RemoteInject::Release()
{
    if (hook_)
        hook_->ReleaseHook();

    if (releaseHook_)
        releaseHook_(hook_);
}

