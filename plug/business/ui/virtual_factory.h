#ifndef _VITUAL_FACTORY_H_
#define _VITUAL_FACTORY_H_

#include <string>
#include <memory>
#include <Windows.h>
#include "main_form.h"
#include <nana/gui/tooltip.hpp>

class temp_tooltip_interface
{
public:
    virtual ~temp_tooltip_interface(){}
    virtual bool tooltip_empty() const = 0;
    virtual int tooltip_size() const = 0;
    virtual void tooltip_text(const std::wstring&) = 0;
    virtual void tooltip_move(const POINT pt, bool ignore) = 0;
    virtual void duration(int) = 0;
};

class temp_factory_interface
{
public:
    virtual ~temp_factory_interface() {}
    virtual temp_tooltip_interface* create() = 0;
    virtual void destory(temp_tooltip_interface*) = 0;
};

template<typename temp_tooltip_window>
class temp_factory : public temp_factory_interface
{
public:
    explicit temp_factory();
    ~temp_factory();

    virtual temp_tooltip_interface* create() override
    {
        return new temp_tooltip_window;
    }

    virtual void destory(temp_tooltip_interface* factory) override
    {
        if (factory)
        {
            delete factory;
            factory = nullptr;
        }
    }
};

class temp_tooltip_form: public temp_tooltip_interface
{
public:
    temp_tooltip_form()
        : temp_tooltip_interface()
        , tip_()
    {
        tip_.reset(new nana::tooltip(MainForm::Get()->handle(), L"sss"));
    }
    ~temp_tooltip_form()
    {

    }

    virtual bool tooltip_empty() const override
    {
        return true;
    }
    virtual int tooltip_size() const override
    {
        return 1;
    }
    virtual void tooltip_text(const std::wstring& str) override
    {

    }
    virtual void tooltip_move(const POINT pt, bool ignore) override
    {

    }
    virtual void duration(int secoode) override
    {

    }

private:
    std::unique_ptr<nana::tooltip> tip_;
};

#endif