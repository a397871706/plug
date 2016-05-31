#ifndef _HOOK_MESSAGE_H_
#define _HOOK_MESSAGE_H_

#include <Windows.h>

#include <base/memory/singleton.h>

#include "hook_type.h"


class HookMessageHelper
{
public:
    ~HookMessageHelper();

    void SetHookListening(MessageListening* listening);
    void ProcessMessage(HWND hWnd, UINT msg, WPARAM w, LPARAM l);

    static HookMessageHelper* GetInstance();

private:
    friend struct DefaultSingletonTraits<HookMessageHelper>;

    HookMessageHelper();
    MessageListening* listening_;

    DISALLOW_COPY_AND_ASSIGN(HookMessageHelper);
};

class HookMessage
{
public:
    HookMessage();
    ~HookMessage();

    bool Acquire();
    void Release();    

private:    
    static LRESULT CALLBACK CallWndProc(int ncode, WPARAM wParam, LPARAM lParam);
    void Logging();

    static HHOOK hook_;
};

#endif