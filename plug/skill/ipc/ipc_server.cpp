#include "ipc_server.h"

#include "ipc_listener.h"

#include <ipc/ipc_channel_win.h>

namespace
{
const char* pipeName = "TestPipe";
}

using IPC::Channel;
using IPC::ChannelHandle;
using IPC::ChannelWin;

TestIPCConnent::TestIPCConnent(TestIPCListener* listener)
    : ipc_channel_(nullptr)
    , listener_(listener)
    , ipc_thread_(new base::Thread("IPCThread"))
    , connent_success_(false)
{
    //ipc_thread_->Start();
}

TestIPCConnent::~TestIPCConnent()
{
    if (ipc_thread_ && ipc_thread_->IsRunning())
        ipc_thread_->Stop();
}

bool TestIPCConnent::Start()
{
    if (!ipc_thread_ && !ipc_thread_->IsRunning())
        return false;

    base::Thread::Options options;
    options.message_loop_type = base::MessageLoop::TYPE_IO;
    ipc_thread_->StartWithOptions(options);
    ipc_thread_->message_loop()->PostTask(
        FROM_HERE,
        base::Bind(&TestIPCConnent::OnState, base::Unretained(this)));

    return true;
}

bool TestIPCConnent::Stop()
{
    if (!ipc_thread_ && !ipc_thread_->IsRunning())
        return false;

    ipc_thread_->message_loop()->PostTask(
        FROM_HERE, base::Bind(&TestIPCConnent::OnStop, base::Unretained(this)));

    return true;
}

void TestIPCConnent::OnState()
{
    if (!connent_success_)
    {
        ChannelHandle handle(pipeName);
        ipc_channel_.reset(new ChannelWin(handle, IPC::Channel::MODE_SERVER,
            listener_, nullptr));
        if (ipc_channel_->Connect())
            connent_success_ = true;

        if (!connent_success_)
        {
            base::MessageLoop::current()->PostTask(
                FROM_HERE, base::Bind(&TestIPCConnent::OnState,
                                      base::Unretained(this)));
        }
    }
}

void TestIPCConnent::OnStop()
{
    if (ipc_channel_)
        ipc_channel_->Close();
}
