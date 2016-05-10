#include "main_form_messageloop.h"
#include "main_form.h"

MainFormMessageLoop::MainFormMessageLoop()
    : main_form_(new MainForm())
{

}

MainFormMessageLoop::~MainFormMessageLoop()
{

}

void MainFormMessageLoop::BeginMainForm()
{

}

void MainFormMessageLoop::EndMainForm()
{

}

void MainFormMessageLoop::MessageLoop()
{
    if (main_form_)
    {
        main_form_->show();
        main_form_->wait_for_this();
    }
}
