#ifndef _MAIN_FORM_DESPATCHER_H_
#define _MAIN_FORM_DESPATCHER_H_

#include <base/message_loop/message_pump_dispatcher.h>

class MainFormDespatcher : public base::MessagePumpDispatcher
{
public:
    MainFormDespatcher();
    ~MainFormDespatcher();

    virtual uint32_t Dispatch(const base::NativeEvent& msg) override;
};

#endif