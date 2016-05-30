#ifndef _THUMBNAIL_TOOLBARS_H_
#define _THUMBNAIL_TOOLBARS_H_

#include <Windows.h>

struct ITaskbarList4;

class ThumbnailToolbar
{
public:
    ThumbnailToolbar();
    ~ThumbnailToolbar();

    bool ThumbBarAddButtons();
    bool ThumbBarUpdateButtons();

private:
    typedef BOOL (WINAPI*PF_ChangeWindowMessageFilter)(UINT, DWORD);
    typedef BOOL (WINAPI*PF_ChangeWindowMessageFilterEX)(HWND, UINT, DWORD, PCHANGEFILTERSTRUCT);
    int WM_TASKBARCREATED;
    int WM_TASKBARBUTTONCREATED;
    std::shared_ptr<ITaskbarList4> taskbar_;
};


#endif