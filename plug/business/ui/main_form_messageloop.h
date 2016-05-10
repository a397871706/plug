#ifndef _MAIN_FORM_MESSAGELOOP_H_
#define _MAIN_FORM_MESSAGELOOP_H_

class MainFormDelegate;

class MainFormMessageLoop
{
public:
    MainFormMessageLoop();
    ~MainFormMessageLoop();

    void BeginMainForm();
    void EndMainForm();
    void MessageLoop();

    MainFormDelegate* delegate_;
};

#endif