#include "thumbnail_toolbars.h"

#include <Shobjidl.h>

#include "main_form.h"
#include "../../../resource.h"

ThumbnailToolbar::ThumbnailToolbar()
{
    ::CoInitialize(NULL);
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

    hr = ptbl->ThumbBarAddButtons(MainForm::Get()->GetHWND(), ARRAYSIZE(thmb), thmb);
    return SUCCEEDED(hr);
}

bool ThumbnailToolbar::ThumbBarUpdateButtons()
{
    return true;
}
