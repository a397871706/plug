#include "../../business/ui/main_form.h"

#include <stdlib.h> 
#include <Winuser.h>

#include "../../third_party/nana/include/nana/gui/programming_interface.hpp"
#include "../../third_party/nana/include/nana/gui/basis.hpp"
#include "../../skill/privilege/promote_privilege.h"
#include "../../business/algorithm/link_game_type.h"
#include "../../business/algorithm/link_game.h"
#include "../../skill/base/app_reg.h"
#include "../../resource.h"
#include "thumbnail_toolbars.h"

#include <nana/gui/widgets/button.hpp>
#include <nana/paint/image.hpp>
#include <nana/gui/timer.hpp>
#include <nana/gui/widgets/checkbox.hpp>
#include <nana/gui/widgets/slider.hpp>
#include <nana/gui/programming_interface.hpp>
#include <nana/gui/tooltip.hpp>
#include <nana/gui/notifier.hpp>
#include <nana/gui/widgets/menu.hpp>
#include <nana/threads/pool.hpp>

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;
using std::placeholders::_4;

namespace
{
const wchar_t* game = L"QQ游戏 - 连连看角色版";
const int clearSecode = 1500;
const int startSecode = 5 * 1000;

void OnMenuItem(const nana::menu::item_proxy& ip)
{
    switch (ip.index())
    {
    case 1:
        break;
    default:
        break;
    }
}
}

MainForm* MainForm::main_form_ = nullptr;

MainForm::~MainForm()
{
    if (main_form_)
    {
        main_form_ = nullptr;
    }
}

MainForm* MainForm::Get()
{
    if (!main_form_)
        main_form_ = new MainForm();

    return main_form_;
}

MainForm::MainForm()
    : nana::form(nana::API::make_center(310, 130),
    nana::appearance(true, true, false, true, true, false, true))
    , start_()
    , current_pt_()
    , link_game_()
    , icon_()
    , timer_()
    , auto_wait_for_()
    , auto_start_()
    , start_timer_()
    , all_clear_()
    , slider_()
    , slider_switch_()
    , tip_()
    , long_start_timer_()
    , tray_icon_()
    , pool_(new nana::threads::pool(3))
{
    caption(L"QQ连连看外挂");
    fgcolor(nana::color(192, 192, 192));
    HWND hWnd = GetHWND();
    this->events().destroy.connect(std::bind(&MainForm::OnDestory, this, _1));
    LONG style = ::GetWindowLong(hWnd, GWL_STYLE);
    ::SetWindowLong(hWnd, GWL_STYLE, style & ~WS_THICKFRAME);
    start_.reset(new nana::button(*this, nana::rectangle(250, 10, 50, 30)));
    start_->events().click.connect(std::bind(&MainForm::OnSingleClick, this, _1));
    start_->caption(L"单消");
    //icon_.reset(new nana::paint::image(L"..\\bin\\QQGame 001.ico"));
    //nana::API::window_icon(*this, *icon_);
    //this->icon(*icon_.get());
    /*HICON hIcon =(HICON)::LoadImage(NULL, L"I:\\plug\\bin\\favicon-20160323043854820.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
    ::SetClassLong(hWnd, GCL_HICON, static_cast<LONG>(reinterpret_cast<LONG_PTR>(hIcon)));
    ::SetClassLong(hWnd, GCL_HICONSM, static_cast<LONG>(reinterpret_cast<LONG_PTR>(hIcon))); // 相当于WM_SETICON*/
    HICON hIcon = reinterpret_cast<HICON>(::LoadImage(GetModuleHandle(NULL),
        MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));
    ::SendMessage(hWnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(hIcon));
    timer_.reset(new nana::timer());
    timer_->elapse(std::bind(&MainForm::OnTimer, this));

    auto_wait_for_.reset(new nana::checkbox(*this, nana::rectangle(20, 16, 80, 20)));
    auto_wait_for_->check(false);
    auto_wait_for_->caption(L"自动挂机");
    auto_wait_for_->transparent(false);
    auto_wait_for_->events().click.connect(std::bind(&MainForm::OnAutoWaitFor, this, _1));

    auto_start_.reset(new nana::checkbox(*this, nana::rectangle(160, 16, 80, 20)));
    auto_start_->check(false);
    auto_start_->caption(L"自动开局");
    auto_start_->transparent(false);
    auto_start_->events().click.connect(std::bind(&MainForm::OnAutoStart, this, _1));

    start_timer_.reset(new nana::timer());
    start_timer_->elapse(std::bind(&MainForm::OnStartTimer, this));

    long_start_timer_.reset(new nana::timer());
    long_start_timer_->elapse(std::bind(&MainForm::OnLongStart, this));

    all_clear_.reset(new nana::button(*this, nana::rectangle(250, 69, 50, 30)));
    all_clear_->caption(L"秒杀");
    all_clear_->events().click.connect(std::bind(&MainForm::OnAllClear, this, _1));

    slider_switch_.reset(new nana::checkbox(*this, nana::rectangle(20, 70, 80, 20)));
    slider_switch_->check(false);
    slider_switch_->caption(L"启动调节器");
    slider_switch_->transparent(false);
    slider_switch_->events().click.connect(std::bind(&MainForm::OnEnableSlider, this, _1));

    slider_.reset(new nana::slider(*this, true));
    slider_->size(nana::size(150, 20));
    slider_->vmax(3000);
    slider_->move(20, 96);
    slider_->events().value_changed.connect(std::bind(&MainForm::OnSliderChangedValue, this, _1));
    slider_->events().mouse_move.connect(std::bind(&MainForm::OnSliderMouseMove, this, _1));
    slider_->enabled(false);

    moust_top_.reset(new nana::checkbox(*this, nana::rectangle(160, 70, 80, 20)));
    moust_top_->caption(L"游戏最前");
    moust_top_->check(false);
    moust_top_->events().click.connect(std::bind(&MainForm::OnMoustTop, this, _1));

    link_game_.reset(new plug::LinkGameEraser(
        std::bind(&MainForm::ClickTwoPoint, this, _1, _2)));

    tray_icon_.reset(new nana::notifier(*this));
    tray_icon_->icon(L"..\\bin\\QQGame 001.ico");
    tray_icon_->text(L"QQ连连看看外挂");
    /*
    nana::threads::pool regPool(1);
    regPool.push(plug::SetAppReg);
    regPool.wait_for_finished();*/

    tray_icon_->events().mouse_down([this](const nana::arg_notifier& arg)
    {
        if (arg.left_button)
        {
            HWND hWnd = GetHWND();
            this->OnForegroundHwnd(hWnd);
        }
        else if (arg.right_button)
        {
            nana::menu* tray_menu = new nana::menu();
            tray_menu->append(L"test", OnMenuItem);
            POINT pt = { 0 };            
            ::GetCursorPos(&pt);
            HMONITOR monitor =
                ::MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST);
            MONITORINFO mi = { 0 };
            mi.cbSize = sizeof(mi);
            POINT p = pt;
            if (monitor && ::GetMonitorInfo(monitor, &mi))
            {
                if (pt.x + tray_menu->item_pixels() > mi.rcWork.right)
                {
                    p.x = pt.x - tray_menu->item_pixels();
                }

                if (pt.y + tray_menu->item_pixels() < mi.rcWork.bottom)
                {
                    p.y = pt.y - tray_menu->size() == 1 ? pt.y - 2 * tray_menu->item_pixels() : pt.y - tray_menu->size() * tray_menu->item_pixels();
                }
                else
                {
                    p.y = pt.y - tray_menu->size() == 1 ? pt.y - 2 * tray_menu->item_pixels() : pt.y - tray_menu->size() * tray_menu->item_pixels();
                }
                //tray_menu->popup(nullptr, pt.x, pt.y);
                tray_menu->popup_await(nullptr, p.x, p.y);
            }
            
            delete tray_menu;
        }
    });

    Init();
}

void MainForm::RunApp()
{
    if (main_form_)
    {
        main_form_->show();
        wait_for_this();
    }
}

void MainForm::OnSingleClick(const nana::arg_click& arg)
{
    OnReadMemoryData();
    if (link_game_)
    {
        link_game_->ClearPair();
        if (IsSquareAllClear() && !start_timer_->started())
            start_timer_->start();
    }
}

void MainForm::OnDestory(const nana::arg_destroy& arg)
{
    if (tip_)
        tip_->close();
}

void MainForm::OnReadMemoryData()
{
    HANDLE handle = GetPrivilege();
    BYTE buffer[11][19] = { 0 };
    void* baseValue = reinterpret_cast<void*>(0x00189F78);
    SIZE_T sz = 0;
    ::ReadProcessMemory(handle, baseValue, reinterpret_cast<void*>(&buffer),
                        11 * 19, &sz);

    if (link_game_)
        link_game_->SetBoardData(buffer);
}

bool MainForm::ClickTwoPoint(const POINT first, const POINT secode)
{
    HWND hWnd = ::FindWindow(NULL, game);
    int lParam = ((first.y * 35 + 192) << 16) + (first.x * 31 + 21);
    ::SendMessage(hWnd, WM_LBUTTONDOWN, 0, lParam);
    ::SendMessage(hWnd, WM_LBUTTONUP, 0, lParam);

    lParam = ((secode.y * 35 + 192) << 16) + (secode.x * 31 + 21);
    ::SendMessage(hWnd, WM_LBUTTONDOWN, 0, lParam);
    ::SendMessage(hWnd, WM_LBUTTONUP, 0, lParam);
    
    return true;
}

void MainForm::OnTimer()
{
    OnReadMemoryData();
    if (link_game_)
    {
        link_game_->ClearPair();
        if (IsSquareAllClear() && !start_timer_->started())
            start_timer_->start();
    }
}

void MainForm::OnAutoWaitFor(const nana::arg_click& arg)
{
    if (!timer_ || !auto_wait_for_)
        return;

    if (auto_wait_for_->checked())
    {
        if (!timer_->started())
        {
            timer_->interval(clearSecode);
            timer_->start();
        }
    }
    else
    {
        if (timer_->started())
        {
            timer_->stop();
        }
    }
}

void MainForm::OnAutoStart(const nana::arg_click& arg)
{
    if (!auto_start_ || !start_timer_)
        return;

    if (auto_start_->checked())
    {
        if (!start_timer_->started())
        {
            start_timer_->interval(startSecode);
            start_timer_->start();
        }
    }
    else
    {
        if (start_timer_->started())
            start_timer_->stop();
    }
}

void MainForm::OnStartTimer()
{
    HANDLE handle = GetPrivilege();
    int isStart = -1;
    SIZE_T readData = 0;
    ::ReadProcessMemory(handle, reinterpret_cast<void*>(0x0008B000),
                        reinterpret_cast<void*>(&isStart), sizeof(int),
                        &readData);
   
    if (isStart == 0) // 准备 = 1 未准备 = 0  不是很精准
    {        
        OnClickStart();
        if (start_timer_->started())
            start_timer_->stop();

        if (long_start_timer_ && long_start_timer_->started())
            long_start_timer_->stop();
    }
    else
    {
        if (long_start_timer_)
        {
            long_start_timer_->interval(1000 * 120);
            long_start_timer_->start();
        }
    }
}

void MainForm::OnLongStart()
{
    OnClickStart();
}

bool MainForm::OnClickStart()
{
    HWND hWnd = ::FindWindow(NULL, game);
    if (hWnd)
    {
        if (!moust_top_->checked())
        {
            OnForegroundHwnd(hWnd);
        }

        Sleep(500);
        RECT  rc = { 0 };
        ::GetWindowRect(hWnd, &rc);
        ::GetCursorPos(&current_pt_);
        POINT pt = { rc.left + 663, rc.top + 564 };
        ::SetCursorPos(pt.x, pt.y);
        mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, NULL);
        Sleep(50);
        mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, NULL);
        ::SetCursorPos(current_pt_.x, current_pt_.y);        
        return true;
    }

    return false;
}

void MainForm::OnAllClear(const nana::arg_click& arg)
{
    OnReadMemoryData();
    if (link_game_)
    {
        link_game_->ClearPair();
        if (IsSquareAllClear() && !start_timer_->started())
            start_timer_->start();
    }
}

void MainForm::OnEnableSlider(const nana::arg_click& arg)
{
    if (!slider_ || !slider_switch_)
        return;
    
    bool checked = slider_switch_->checked();
    slider_->enabled(checked);
}

void MainForm::OnSliderChangedValue(const nana::arg_slider& arg)
{
    if (!auto_wait_for_ || !timer_)
        return;

    if (auto_wait_for_->checked() && timer_->started())
    {
        timer_->interval(arg.widget.value() * 4);
    }
}

void MainForm::OnSliderMouseMove(const nana::arg_mouse& arg)
{
    if (!slider_)
        return;

    RECT rc = { slider_->pos().x, slider_->pos().y, 
                slider_->pos().x + slider_->size().width, 
                slider_->pos().y + slider_->size().height };
    POINT pt = {};
    ::GetCursorPos(&pt);
    ::ScreenToClient(reinterpret_cast<HWND>(nana::API::root(this->handle())), &pt);      
    if (::PtInRect(&rc, pt))
    {
        if (!tip_)
            tip_.reset(new nana::tooltip());

        std::wostringstream os;
        os << (pt.x - slider_->pos().x) * 20;
        tip_->show(*this, nana::point(pt.x + 10, pt.y - 2), os.str(), 1000);
    }
}

void MainForm::OnMoustTop(const nana::arg_click& arg)
{
    if (!moust_top_)
        return;

    HWND hWnd = ::FindWindow(NULL, game);    
    if (moust_top_->checked())
    {
        moust_top_->caption(L"取消最前");
        ::SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, 
                       SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
    }
    else
    {   
        moust_top_->caption(L"游戏最前");
        ::SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, 
                       SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
    }
}

HANDLE MainForm::GetPrivilege()
{
    HWND hWnd = ::FindWindow(NULL, game);
    DWORD processId = 0;
    ::GetWindowThreadProcessId(hWnd, &processId);
    HANDLE handle = plug::OpenProcess(processId);
    if (!handle)
    {
        plug::PromotePrivilege();
        handle = plug::OpenProcess(processId);
    }

    return handle;
}

void MainForm::OnForegroundHwnd(HWND hWnd)
{
    if (::IsIconic(hWnd))
    {
        ::ShowWindow(hWnd, SW_RESTORE);
    }

    ::SetForegroundWindow(hWnd);
}

void MainForm::OnStopTimerProcess()
{
    HANDLE handle = GetPrivilege();
    BYTE nop[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
    SIZE_T size = 0;
    ::WriteProcessMemory(handle, reinterpret_cast<void*>(0x0041DF1D),
                         reinterpret_cast<void*>(nop), 6, &size);
}

bool MainForm::IsSquareAllClear()
{
    HANDLE handle = GetPrivilege();
    BYTE isAllClear = 0;
    SIZE_T readData = 0;
    ::ReadProcessMemory(handle, reinterpret_cast<void*>(0x00174DDC),
                        reinterpret_cast<void*>(&isAllClear), sizeof(BYTE),
                        &readData);
    if (isAllClear == 0)
        return true;

    return false;
}

HWND MainForm::GetHWND()
{
    return reinterpret_cast<HWND>(nana::API::root(this->handle()));
}

void MainForm::Init()
{
    if (!thumb_)
        thumb_.reset(new ThumbnailToolbar());

    thumb_->ThumbBarAddButtons();
}
