#ifndef _MAIN_FORM_MESSAGELOOP_H_
#define _MAIN_FORM_MESSAGELOOP_H_

#include <memory>

class MainForm;

class MainFormMessageLoop
{
public:
    MainFormMessageLoop();
    ~MainFormMessageLoop();

    void BeginMainForm();
    void EndMainForm();
    void MessageLoop();

private:
    std::shared_ptr<MainForm> main_form_;
};

#endif