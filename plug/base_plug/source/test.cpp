#include "../include/test.h"

Test::Test()
    : i_(0)
    , str_()
{

}

Test::Test(int i, const std::wstring& str)
    : i_(i)
    , str_(str)
{

}

Test::~Test()
{

}

int Test::GetInt() const
{
    return i_;
}

std::wstring* Test::GetString()
{
    std::wstring* str = new std::wstring();
    str->assign(str_);
    return str;
}

void Test::ReleaseString(std::wstring* str)
{
    if (str)
        delete str;
}
