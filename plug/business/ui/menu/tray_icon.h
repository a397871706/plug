#ifndef _TRAY_ICON_H_
#define  _TRAY_ICON_H_

#include <memory>

namespace nana
{
class notifier;
struct arg_notifier;
}

struct ITaskbarList4;

class MainFormDelegate;

class TrayIcon
{
public:
    TrayIcon();
    ~TrayIcon();

    void SetIcon();
    void AddTrayIcon();
    bool ThumbBarAddButtons();
    bool ThumbBarUpdateButtons();

private:
    void OnTrayIconNotify(const nana::arg_notifier& arg);

    std::unique_ptr<nana::notifier> tray_icon_;
    std::shared_ptr<ITaskbarList4> taskbar_;
    MainFormDelegate* delegate_;
};

#endif