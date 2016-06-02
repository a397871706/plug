#include "main_form_messageloop.h"

#include <base/at_exit.h>
#include <base/run_loop.h>
#include <base/message_loop/message_loop.h>
#include <base/threading/platform_thread.h>

#include "main_form_despatcher.h"
#include "main_form.h"

MainFormMessageLoop::MainFormMessageLoop()
    : at_exit_manager_(nullptr)
    , message_loop_ui_(nullptr)
{

}

MainFormMessageLoop::~MainFormMessageLoop()
{
}

void MainFormMessageLoop::Shutdown()
{
    auto main_form_delegate = MainFormDelegate::Get();
    if (main_form_delegate)
    {
        main_form_delegate->Release();
        delete main_form_delegate;
        main_form_delegate = nullptr;
    }
}

void MainFormMessageLoop::Run()
{
    MainFormDespatcher despatcher;
    base::RunLoop loop(&despatcher);
    loop.Run();
}

void MainFormMessageLoop::Initialize()
{
    at_exit_manager_.reset(new base::AtExitManager());
    message_loop_ui_.reset(new base::MessageLoopForUI());
    auto main_form = MainFormDelegate::Get();
    base::PlatformThread::SetName("plug线程");
    main_form->Show();
}
