#include <Windows.h>

#include "business/ui/main_form_messageloop.h"


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    LPWSTR lpCmdLine, int nShowCmd)
{
    MainFormMessageLoop message_loop_;
    message_loop_.BeginMainForm();
    message_loop_.MessageLoop();
    message_loop_.EndMainForm();
    return 0;
}