#include "ipc_server.h"

#include <ipc/ipc_channel.h>

namespace
{
const char* pipeName = "TestPipe";
}

using IPC::Channel;
using IPC::ChannelHandle;

TestIPCConnent::TestIPCConnent()
    : ipc_channel_(nullptr)
{

}

TestIPCConnent::~TestIPCConnent()
{

}

void TestIPCConnent::Start()
{
    ChannelHandle handle(pipeName);
    ipc_channel_ = Channel::CreateNamedServer(handle,)
}

void TestIPCConnent::Stop()
{

}
