#include "hook_message.h"

#include <base/threading/platform_thread.h>
#include <base/logging.h>
#include <base/strings/string_number_conversions.h>

HHOOK HookMessage::hook_ = NULL;

HookMessage::HookMessage()
{

}

HookMessage::~HookMessage()
{
    hook_ = NULL;
}

bool HookMessage::Acquire()
{        
    hook_ = ::SetWindowsHookEx(WH_CALLWNDPROCRET, CallWndProc, NULL,
                               GetCurrentThreadId());
    if (!hook_)
    {
        Logging();
        return false;
    }

    return true;
}

void HookMessage::Release()
{
    if (!::UnhookWindowsHookEx(hook_))
    {
        Logging();
    }
}

LRESULT CALLBACK HookMessage::CallWndProc(int ncode, WPARAM wParam, LPARAM lParam)
{
    HookMessageHelper* helper = HookMessageHelper::GetInstance();
    CWPRETSTRUCT* l = reinterpret_cast<CWPRETSTRUCT*>(lParam);
    if (l)
        helper->ProcessMessage(l->hwnd, l->message, l->wParam, l->lParam);

    return ::CallNextHookEx(hook_, ncode, wParam, lParam);
}

void HookMessage::Logging()
{
    LOG(INFO) << (base::PlatformThread::GetName() ?
                  base::PlatformThread::GetName() :
                  base::UintToString(GetCurrentThreadId()))
                  << base::UintToString(GetLastError());
}

HookMessageHelper::~HookMessageHelper()
{

}

void HookMessageHelper::SetHookListening(MessageListening* listening)
{
    listening_ = listening;
}

void HookMessageHelper::ProcessMessage(HWND hWnd, UINT msg, WPARAM w, LPARAM l)
{
    if (listening_)
        listening_->ProcessMessage(hWnd, msg, w, l);
}

HookMessageHelper* HookMessageHelper::GetInstance()
{
    return Singleton<HookMessageHelper>::get();
}

HookMessageHelper::HookMessageHelper()
    : listening_(nullptr)
{

}