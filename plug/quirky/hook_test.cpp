#include "hook_test.h"
#include "resource.h"

HookInterface* hInterface = nullptr;
extern HMODULE gModule;


extern "C" EXPORT HookInterface* __stdcall SetHook()
{
    if (!hInterface)
        hInterface = new HookEx();

    return hInterface;
}

extern "C" EXPORT void __stdcall Release(HookInterface* hook)
{
    if (hook != nullptr && hook == hInterface)
    {
        delete hInterface;
        hInterface = nullptr;
    }
}

HHOOK HookEx::hook_ = NULL;

HookEx::HookEx()
    : HookInterface()
{

}

HookEx::~HookEx()
{

}

BOOL HookEx::SetHook(DWORD threadID)
{
    hook_ = ::SetWindowsHookEx(WH_GETMESSAGE, GetMsgProc, gModule, threadID);
    return hook_ != NULL;
}

BOOL HookEx::ReleaseHook()
{
    return ::UnhookWindowsHookEx(hook_);
}

BOOL HookEx::SendMessage(UINT msg, WPARAM w, LPARAM l)
{
    return TRUE;
}




LRESULT CALLBACK HookEx::GetMsgProc(int code, WPARAM wParam, LPARAM lParam)
{
    if (code < 0)
    {
        ::CallNextHookEx(hook_, code, wParam, lParam);
        return 0;
    }

    MSG* msg = reinterpret_cast<MSG*>(lParam);
    if (msg)
    {
        if (msg->message == WM_KEYDOWN)
        {
            if ((msg->wParam == VK_HOME) && ((msg->lParam << 30) & 0x40000000))
            {
                ::DialogBoxParam(gModule, MAKEINTRESOURCE(IDD_DIALOG1), NULL,
                                 DilogBox, NULL);
            }
        }
    }

    return ::CallNextHookEx(hook_, code, wParam, lParam);
}

INT_PTR CALLBACK HookEx::DilogBox(HWND h, UINT m, WPARAM w, LPARAM l)
{
    return 0;
}
