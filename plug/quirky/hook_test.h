#ifndef _HOOK_TEST_H_
#define _HOOK_TEST_H_

#include <Windows.h>

#include "export_macro.h"

class HookInterface
{
public:
    virtual ~HookInterface() { }
    virtual BOOL SetHook(DWORD threadID) = 0;
    virtual BOOL ReleaseHook() = 0;
    virtual BOOL SendMessage(UINT msg, WPARAM w, LPARAM l) = 0;

};

extern "C" EXPORT HookInterface* __stdcall SetHook();

extern "C" EXPORT void __stdcall Release(HookInterface* hook);

class HookEx : public HookInterface
{
public:
    HookEx();
    ~HookEx();

    virtual BOOL SetHook(DWORD threadID) override;
    virtual BOOL ReleaseHook() override;
    virtual BOOL SendMessage(UINT msg, WPARAM w, LPARAM l) override;

private:
    static LRESULT CALLBACK GetMsgProc(int code, WPARAM wParam, LPARAM lParam);

    static HHOOK hook_;
};

#endif