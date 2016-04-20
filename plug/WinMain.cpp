#include <Windows.h>

#include "business/ui/main_form.h"


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    LPWSTR lpCmdLine, int nShowCmd)
{
    MainFormDelegate main_form_delegate;
    main_form_delegate.BeginMainForm();
    main_form_delegate.MessageLoop();
    main_form_delegate.EndMainForm();
    return 0;
}