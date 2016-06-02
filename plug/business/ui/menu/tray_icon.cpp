#include "tray_icon.h"

#include <Shobjidl.h>

#include <nana/gui/notifier.hpp>
#include <nana/gui/widgets/menu.hpp>

#include "../main_form.h"
#include "../../../resource.h"

namespace
{
const wchar_t* tray_tips = L"QQ连连看看外挂";
const wchar_t* tray_img_path = L"..\\bin\\QQGame 001.ico";
}

TrayIcon::TrayIcon()
    : tray_icon_()
    , taskbar_()
    , delegate_()
{
    ::CoInitialize(NULL);
    delegate_ = MainFormDelegate::Get();
    // registerwindowmessage
}

TrayIcon::~TrayIcon()
{
    ::CoUninitialize();
}

void TrayIcon::AddTrayIcon()
{
    if (!tray_icon_)
    {
        tray_icon_.reset(new nana::notifier(delegate_->GetHandle()));
    }

    tray_icon_->icon(tray_img_path);
    tray_icon_->text(tray_tips);
    tray_icon_->events().mouse_down(std::bind(&TrayIcon::OnTrayIconNotify, this,
                                              std::placeholders::_1));
}

void TrayIcon::OnTrayIconNotify(const nana::arg_notifier& arg)
{
    if (arg.left_button)
    {
        if (delegate_)
            delegate_->ForegroundHwnd();
    }
    else if (arg.right_button)
    {
        nana::menu tray_menu;
        auto menu_handle = [&tray_menu](const nana::menu::item_proxy& ip) -> void
        {
            switch (ip.index())
            {
                case 0:
                {
                }
                break;
            default:
                break;
            }
        };

        tray_menu.append(L"游戏最前", menu_handle);
        tray_menu.append_splitter();
        tray_menu.append(L"自动挂机", menu_handle);
        tray_menu.append(L"自动开局", menu_handle);
        tray_menu.check_style(0, nana::menu::checks::option);

        POINT pt = { 0 };
        ::GetCursorPos(&pt);
        HMONITOR monitor = ::MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST);
        MONITORINFO mi = { 0 };
        mi.cbSize = sizeof(mi);
        POINT p = pt;
        if (monitor && ::GetMonitorInfo(monitor, &mi))
        {
            if (static_cast<LONG>(pt.x + tray_menu.item_pixels()) > mi.rcWork.right)
            {
                p.x = pt.x - tray_menu.item_pixels();
            }

            if (static_cast<LONG>(pt.y + tray_menu.item_pixels()) < mi.rcWork.bottom)
            {
                p.y = pt.y - tray_menu.size() == 1 ? pt.y - 2 * tray_menu.item_pixels() : pt.y - tray_menu.size() * tray_menu.item_pixels();
            }
            else
            {
                p.y = pt.y - tray_menu.size() == 1 ? pt.y - 2 * tray_menu.item_pixels() : pt.y - tray_menu.size() * tray_menu.item_pixels();
            }

            tray_menu.popup_await(nullptr, p.x, p.y);
        }
    }
}

bool TrayIcon::ThumbBarAddButtons()
{
    HINSTANCE hInstance = GetModuleHandle(NULL);
    THUMBBUTTON thmb[2] = {};
    thmb[0].dwMask = THB_ICON | THB_TOOLTIP | THB_FLAGS;
    thmb[0].iId = 0;
    thmb[0].hIcon = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2));
    thmb[0].dwFlags = THBF_ENABLED;
    wcscpy(&thmb[0].szTip[0], L"自动挂机");

    thmb[1].dwMask = THB_ICON | THB_TOOLTIP | THB_FLAGS;
    thmb[1].iId = 1;
    thmb[1].hIcon = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON3));
    thmb[1].dwFlags = THBF_ENABLED;
    wcscpy(&thmb[1].szTip[0], L"自动开局");

    ITaskbarList4* ptbl = nullptr;
    HRESULT hr = CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER,
                                  IID_PPV_ARGS(&ptbl));
    taskbar_.reset(ptbl);
    if (!SUCCEEDED(hr))
        return false;

    ptbl->HrInit();

    hr = ptbl->ThumbBarAddButtons(delegate_->GetHWND(), ARRAYSIZE(thmb), thmb);
    ptbl->Release();
    return SUCCEEDED(hr);
}

bool TrayIcon::ThumbBarUpdateButtons()
{
    HINSTANCE hInstance = GetModuleHandle(NULL);
    THUMBBUTTON thmb[2] = {};
    thmb[0].dwMask = THB_ICON | THB_TOOLTIP | THB_FLAGS;
    thmb[0].iId = 0;
    thmb[0].hIcon = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2));
    thmb[0].dwFlags = THBF_ENABLED;
    wcscpy(&thmb[0].szTip[0], L"自动挂机");

    thmb[1].dwMask = THB_ICON | THB_TOOLTIP | THB_FLAGS;
    thmb[1].iId = 1;
    thmb[1].hIcon = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON3));
    thmb[1].dwFlags = THBF_ENABLED;
    wcscpy(&thmb[1].szTip[0], L"自动开局");


    HRESULT hr = S_OK;
    if (taskbar_)
        hr = taskbar_->ThumbBarUpdateButtons(delegate_->GetHWND(),
                                             ARRAYSIZE(thmb), thmb);
    return SUCCEEDED(hr);
}

void TrayIcon::SetIcon()
{
    //icon_.reset(new nana::paint::image(L"..\\bin\\QQGame 001.ico"));
    //nana::API::window_icon(*this, *icon_);
    //this->icon(*icon_.get());
    /*HICON hIcon =(HICON)::LoadImage(NULL, L"I:\\plug\\bin\\favicon-20160323043854820.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
    ::SetClassLong(hWnd, GCL_HICON, static_cast<LONG>(reinterpret_cast<LONG_PTR>(hIcon)));
    ::SetClassLong(hWnd, GCL_HICONSM, static_cast<LONG>(reinterpret_cast<LONG_PTR>(hIcon))); // 相当于WM_SETICON*/

    // 需要32位 否则不清晰
    HICON hIcon = reinterpret_cast<HICON>(::LoadImage(GetModuleHandle(NULL),
        MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));
    ::SendMessage(delegate_->GetHWND(), WM_SETICON, ICON_SMALL,
                  reinterpret_cast<LPARAM>(hIcon));
}
