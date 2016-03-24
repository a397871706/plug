#ifndef _MAIN_FORM_H_
#define _MAIN_FORM_H_

#include <memory>
#include <Windows.h>

#include <nana/gui/widgets/form.hpp>

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

class MainForm: public nana::form
{
public:    
    virtual ~MainForm();

    static MainForm*  Get();

    void RunApp();

private:
    MainForm();
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

    static void aa();

    static MainForm* main_form_;
    std::unique_ptr<nana::button> start_;
    std::unique_ptr<nana::paint::image> icon_;
    std::unique_ptr<nana::timer> timer_;
    std::unique_ptr<nana::timer> start_timer_;
    std::unique_ptr<nana::timer> long_start_timer_;
    std::unique_ptr<nana::checkbox> auto_wait_for_;
    std::unique_ptr<nana::checkbox> auto_start_;
    std::unique_ptr<nana::button> all_clear_;
    std::unique_ptr<nana::slider> slider_;
    std::unique_ptr<nana::checkbox> slider_switch_;
    std::unique_ptr<nana::checkbox> moust_top_;
    POINT current_pt_;
    std::unique_ptr<plug::LinkGame> link_game_;
    std::unique_ptr<nana::tooltip> tip_;
    std::unique_ptr<nana::threads::pool> pool_;
};


#endif //_MAIN_FORM_H_