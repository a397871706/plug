#ifndef _IPC_SERVER_H_
#define _IPC_SERVER_H_

namespace IPC
{
class Channel;
}

class TestIPCConnent
{
public:
    TestIPCConnent();
    ~TestIPCConnent();

    void Start();
    void Stop();

private:
    scoped_ptr<IPC::Channel> ipc_channel_;
};

#endif