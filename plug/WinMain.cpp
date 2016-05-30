#include <Windows.h>

#include "business/ui/main_form_messageloop.h"


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    LPWSTR lpCmdLine, int nShowCmd)
{
    MainFormMessageLoop message_loop_;
    message_loop_.Initialize();    
    message_loop_.Run();
    message_loop_.Shutdown();    
    return 0;
}