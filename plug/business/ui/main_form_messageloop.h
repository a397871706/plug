#ifndef _MAIN_FORM_MESSAGELOOP_H_
#define _MAIN_FORM_MESSAGELOOP_H_

#include <base/memory/scoped_ptr.h>

namespace base
{
class AtExitManager;
class MessageLoopForUI;
}

class MainFormDelegate;
class MainFormDespatcher;

class MainFormMessageLoop
{
public:
    MainFormMessageLoop();
    ~MainFormMessageLoop();

    void Initialize();
    void Run();
    void Shutdown();    

private:   
    scoped_ptr<base::AtExitManager> at_exit_manager_;
    scoped_ptr<base::MessageLoopForUI> message_loop_ui_;
};

#endif