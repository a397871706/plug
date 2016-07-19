#ifndef _TEST_IPC_SERVER_DELEGATE_H_
#define _TEST_IPC_SERVER_DELEGATE_H_

#include <base\memory\scoped_ptr.h>
#include <base\memory\ref_counted.h>
#include <base\single_thread_task_runner.h>

#include "ipc_listener.h"



class MainForm;
class TestIPCConnent;
class TestIPCListener;

class TestIPCServerDelegate : public TestIPCListener::Delegate
{
public:
    TestIPCServerDelegate(MainForm* mainform);
    ~TestIPCServerDelegate();

    void Start();
    void Stop();
    
protected:
    virtual void OnStart() override;
    virtual void OnStop() override;

private:
    MainForm* main_from_;
    scoped_ptr<TestIPCListener> listener_;
    scoped_ptr<TestIPCConnent> connent_;
    scoped_refptr<base::SingleThreadTaskRunner> main_thread_;
};

#endif