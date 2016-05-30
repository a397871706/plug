#include "thumbnail_toolbars.h"

#include <Shobjidl.h>

#include "main_form.h"
#include "../../../resource.h"

ThumbnailToolbar::ThumbnailToolbar()
    : WM_TASKBARCREATED(0)
    , WM_TASKBARBUTTONCREATED(0)
    , taskbar_()
{
    ::CoInitialize(NULL);
    WM_TASKBARCREATED = ::RegisterWindowMessage(L"TaskbarCreated");
    WM_TASKBARBUTTONCREATED = ::RegisterWindowMessage(L"TaskbarButtonCreated");

    HMODULE hUser32 = ::GetModuleHandle(L"USER32");
    PF_ChangeWindowMessageFilter fc = 
        reinterpret_cast<PF_ChangeWindowMessageFilter>(::GetProcAddress(hUser32,
                                                       "ChangeWindowMessageFilter"));
    PF_ChangeWindowMessageFilterEX fc_ex =
        reinterpret_cast<PF_ChangeWindowMessageFilterEX>(::GetProcAddress(hUser32,
                                                         "ChangeWindowMessageFilterEx"));
    if (fc)
        fc(WM_TASKBARCREATED, MSGFLT_ADD);

    if (fc_ex)
        fc_ex(MainForm::Get()->GetHWND(), WM_TASKBARCREATED, MSGFLT_ALLOW, NULL);
}

ThumbnailToolbar::~ThumbnailToolbar()
{
    ::CoUninitialize();
}

bool ThumbnailToolbar::ThumbBarAddButtons()
{
    HINSTANCE hInstance = GetModuleHandle(NULL);
    THUMBBUTTON thmb[2] = { };
    thmb[0].dwMask = THB_ICON | THB_TOOLTIP | THB_FLAGS;
    thmb[0].iId = 0;
    thmb[0].hIcon = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2));
    thmb[0].dwFlags = THBF_DISMISSONCLICK;
    wcscpy(&thmb[0].szTip[0], L"自动挂机");

    thmb[1].dwMask = THB_ICON | THB_TOOLTIP;
    thmb[1].iId = 1;
    thmb[1].hIcon = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON3));
    wcscpy(&thmb[1].szTip[0], L"自动开局");

    ITaskbarList4* ptbl = nullptr;
    HRESULT hr = CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER,
                                  IID_PPV_ARGS(&ptbl));
    
    if (!SUCCEEDED(hr))
        return false;

    taskbar_.reset(ptbl);

    taskbar_->HrInit();

    hr = ptbl->ThumbBarAddButtons(MainForm::Get()->GetHWND(), ARRAYSIZE(thmb), thmb);
    return SUCCEEDED(hr);
}

bool ThumbnailToolbar::ThumbBarUpdateButtons()
{
    return true;
}
