#include "ipc_server.h"

#include "ipc_listener.h"


namespace
{
const char* pipeName = "TestPipe";
}

using IPC::Channel;
using IPC::ChannelHandle;

TestIPCConnent::TestIPCConnent()
    : ipc_channel_(nullptr)
    , listener_()
    , ipc_thread_(new base::Thread("IPCThread"))
{
    ipc_thread_->Start();
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

    if (!listener_)
        listener_.reset(new TestIPCListener());
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
    ChannelHandle handle(pipeName);
    ipc_channel_ = Channel::CreateNamedServer(handle, listener_.get(), nullptr);
    if (ipc_channel_)
    {
        ipc_channel_->Connect();
    }
}

void TestIPCConnent::OnStop()
{
    if (ipc_channel_)
        ipc_channel_->Close();
}
