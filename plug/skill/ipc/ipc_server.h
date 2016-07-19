#ifndef _IPC_SERVER_H_
#define _IPC_SERVER_H_

#include <memory>
#include <ipc/ipc_channel.h>
#include <base/threading/thread.h>

namespace IPC
{
class ChannelWin;
}

namespace base
{
class Thread;
}

class TestIPCListener;

class TestIPCConnent
{
public:
    TestIPCConnent(TestIPCListener* listener);
    ~TestIPCConnent();

    bool Start();
    bool Stop();
    void Send();

private:
    void OnState();
    void OnStop();

    scoped_ptr<IPC::ChannelWin> ipc_channel_;
    TestIPCListener* listener_;
    scoped_ptr<base::Thread> ipc_thread_;
    bool connent_success_;
};

#endif