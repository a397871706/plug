#include "ipc_server_delegate.h"

#include <base/thread_task_runner_handle.h>

#include "ipc_server.h"
#include "../../business/ui/main_form.h"

TestIPCServerDelegate::TestIPCServerDelegate(MainForm* mainform)
    : main_from_(mainform)
    , listener_(new TestIPCListener(this))
    , connent_(new TestIPCConnent(listener_.get()))
    , main_thread_(base::ThreadTaskRunnerHandle::Get())
{

}

TestIPCServerDelegate::~TestIPCServerDelegate()
{

}

void TestIPCServerDelegate::Start()
{
    connent_->Start();
}

void TestIPCServerDelegate::Stop()
{
    connent_->Stop();
}

void TestIPCServerDelegate::OnStart()
{
    if (!main_thread_->BelongsToCurrentThread())
    {
        main_thread_->PostTask(FROM_HERE, 
                               base::Bind(&TestIPCServerDelegate::OnStart,
                                          base::Unretained(this)));
        return;
    }

    main_from_->TestOnStart();
}

void TestIPCServerDelegate::OnStop()
{
    if (!main_thread_->BelongsToCurrentThread())
    {
        main_thread_->PostTask(FROM_HERE,
                               base::Bind(&TestIPCServerDelegate::OnStart,
                               base::Unretained(this)));
        return;
    }

    main_from_->TestOnStop();
}
