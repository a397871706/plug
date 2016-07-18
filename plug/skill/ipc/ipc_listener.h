#ifndef _IPC_LISTENER_H_
#define _IPC_LISTENER_H_

#include <ipc/ipc_listener.h>

namespace IPC
{
class Listener;
}


class TestIPCListener : public IPC::Listener
{
public:
    TestIPCListener();
    ~TestIPCListener();

protected:
    virtual bool OnMessageReceived(const IPC::Message& message) override;
};

#endif
