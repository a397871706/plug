#ifndef _IPC_SERVER_H_
#define _IPC_SERVER_H_

#include <memory>
#include <ipc/ipc_channel.h>
#include <base/threading/thread.h>

namespace IPC
{
class Channel;
}

namespace base
{
class Thread;
}

class TestIPCListener;

class TestIPCConnent
{
public:
    TestIPCConnent();
    ~TestIPCConnent();

    bool Start();
    bool Stop();

private:
    void OnState();
    void OnStop();

    scoped_ptr<IPC::Channel> ipc_channel_;
    std::unique_ptr<TestIPCListener> listener_;
    scoped_ptr<base::Thread> ipc_thread_;
};

#endif