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
