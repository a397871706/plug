#include <Windows.h>

#include "business/ui/main_form.h"


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    LPWSTR lpCmdLine, int nShowCmd)
{
    MainForm* main_form_ = MainForm::Get();
    if (main_form_)
        main_form_->RunApp();
   
    delete main_form_;
    main_form_ = nullptr;
    return 0;
}