#include "ipc_listener.h"

TestIPCListener::TestIPCListener()
{

}

TestIPCListener::~TestIPCListener()
{

}

bool TestIPCListener::OnMessageReceived(const IPC::Message& message)
{
    return true;
}
