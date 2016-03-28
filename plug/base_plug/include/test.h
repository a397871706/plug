#ifndef _TEST_H_
#define _TEST_H_

#include <string>
#include <vector>

#include "base_export.h"

// https://support.microsoft.com/en-us/kb/168958

template class BASE_EXPORT std::basic_string <wchar_t>;

class BASE_EXPORT Test
{
public:
    Test();
    explicit Test(int i, const std::wstring& str);
    ~Test();

    int GetInt() const;

    std::wstring* GetString();

    void ReleaseString(std::wstring* str);

private:
    int i_;
    std::wstring str_;
};

#endif