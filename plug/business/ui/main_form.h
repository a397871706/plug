#ifndef _MAIN_FORM_H_
#define _MAIN_FORM_H_

#include <memory>
#include <Windows.h>

#include <nana/gui/widgets/form.hpp>
#include <base/memory/ref_counted.h>

#include "../../skill/hook/hook_message.h"
#include "../../skill/hook/hook_type.h"

namespace plug
{
class LinkGame;
}

namespace nana
{
class button;
class Image;
class timer;
class checkbox;
class slider;
struct arg_slider;
class tooltip;

namespace threads
{
class pool;
}
}

namespace base
{
class MessageLoop;
}

class MainForm;
class TrayIcon;
class TestIPCServerDelegate;

class MainFormDelegate : public MessageListening
{
public:
    ~MainFormDelegate();

    static MainFormDelegate* Get();
    void Release();
    void Show();

    HWND GetHWND();
    void ForegroundHwnd();
    nana::window GetHandle();

protected:
    virtual void ProcessMessage(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
        override;

private:
    MainFormDelegate();

    static MainFormDelegate* delegate_;
    std::unique_ptr<MainForm> mainform_;
};

class MainForm: public nana::form
{
public:
    MainForm();
    virtual ~MainForm();

    HWND GetHWND();
    void ForegroundHwnd();

    void Init();

    void TestOnStart();
    void TestOnStop();

protected:
    void wait_for_this();

private:
    friend class MainFormDelegate;

    void OnSingleClick(const nana::arg_click& arg);
    void OnReadMemoryData();
    void OnDestory(const nana::arg_destroy& arg);
    bool ClickTwoPoint(const POINT first, const POINT secode);
    void OnTimer();
    void OnStartTimer();
    void OnAutoWaitFor(const nana::arg_click& arg);
    void OnAutoStart(const nana::arg_click& arg);
    bool OnClickStart();
    void OnAllClear(const nana::arg_click& arg);
    void OnEnableSlider(const nana::arg_click& arg);
    void OnSliderChangedValue(const nana::arg_slider& arg);
    void OnSliderMouseMove(const nana::arg_mouse& arg);
    void OnMoustTop(const nana::arg_click& arg);
    void OnStopTimerProcess();
    bool IsSquareAllClear();
    void OnLongStart();

    void OnForegroundHwnd(HWND hWnd);
    HANDLE GetPrivilege();
    void OnGameAlgorithm();

    std::unique_ptr<nana::button> start_;
    std::unique_ptr<nana::timer> timer_;
    std::unique_ptr<nana::timer> start_timer_;
    std::unique_ptr<nana::timer> long_start_timer_;
    std::unique_ptr<nana::checkbox> auto_wait_for_;
    std::unique_ptr<nana::checkbox> auto_start_;
    std::unique_ptr<nana::button> all_clear_;
    std::unique_ptr<nana::slider> slider_;
    std::unique_ptr<nana::checkbox> slider_switch_;
    std::unique_ptr<nana::checkbox> moust_top_;
    std::unique_ptr<plug::LinkGame> link_game_;
    std::unique_ptr<nana::tooltip> tip_;
    std::unique_ptr<nana::threads::pool> pool_;
    std::unique_ptr<TrayIcon> trayicon_;
    std::unique_ptr<TestIPCServerDelegate> test_ipc_delegate_;
    POINT current_pt_;
    base::MessageLoop* ui_message_loop_;
    HookMessage hook_message_;
};


#endif //_MAIN_FORM_H_