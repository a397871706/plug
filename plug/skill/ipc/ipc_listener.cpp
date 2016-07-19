#include "ipc_listener.h"
#include <ipc\ipc_message_macros.h>

#include "test_ipc_message.h"

TestIPCListener::TestIPCListener(Delegate* delegate)
    : delegate_(delegate)
{

}

TestIPCListener::~TestIPCListener()
{

}

bool TestIPCListener::OnMessageReceived(const IPC::Message& message)
{
    bool handled = true;
    IPC_BEGIN_MESSAGE_MAP(TestIPCListener, message)
        IPC_MESSAGE_HANDLER(TestStart, OnStart)
        IPC_MESSAGE_HANDLER(TestStop, OnStop)
        IPC_MESSAGE_UNHANDLED(handled = false)
    IPC_END_MESSAGE_MAP()
    return handled;
}

void TestIPCListener::OnStart()
{
    if (delegate_)
        delegate_->OnStart();
}

void TestIPCListener::OnStop()
{
    if (delegate_)
        delegate_->OnStop();
    
}
