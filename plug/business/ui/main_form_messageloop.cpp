#include "main_form_messageloop.h"
#include "main_form.h"

MainFormMessageLoop::MainFormMessageLoop()
    : delegate_()
{

}

MainFormMessageLoop::~MainFormMessageLoop()
{

}

void MainFormMessageLoop::BeginMainForm()
{
    delegate_ = MainFormDelegate::Get();
    delegate_->Init();
}

void MainFormMessageLoop::EndMainForm()
{
    if (delegate_)
    {
        delegate_->Release();
        delete delegate_;
        delegate_ = nullptr;
    }
}

void MainFormMessageLoop::MessageLoop()
{
    if (delegate_)
        delegate_->MessageLoop();
}
