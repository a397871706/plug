#include "main_form_despatcher.h"

MainFormDespatcher::MainFormDespatcher()
    : base::MessagePumpDispatcher()
{

}

MainFormDespatcher::~MainFormDespatcher()
{

}

uint32_t MainFormDespatcher::Dispatch(const base::NativeEvent& msg)
{
    ::TranslateMessage(&msg);
    ::DispatchMessage(&msg);
    return POST_DISPATCH_NONE;
}
