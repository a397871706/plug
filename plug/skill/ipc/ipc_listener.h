#ifndef _IPC_LISTENER_H_
#define _IPC_LISTENER_H_

#include <ipc/ipc_listener.h>

namespace IPC
{
class Listener;
}

class TestIPCServerDelegate;

class TestIPCListener : public IPC::Listener
{
public:
    class Delegate
    {
    public:
        virtual ~Delegate() { }
        virtual void OnStart() = 0;
        virtual void OnStop() = 0;
    };

    TestIPCListener(Delegate * delegate);
    ~TestIPCListener();

protected:
    virtual bool OnMessageReceived(const IPC::Message& message) override;

private:
    void OnStart();
    void OnStop();

    Delegate* delegate_;
};

#endif
