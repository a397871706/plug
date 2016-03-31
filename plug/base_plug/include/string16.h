#ifndef _BASE_STRING16_H_
#define _BASE_STRING16_H_

#include <stdio.h>
#include <string>

#include "base_export.h"
#include "basictypes.h"

#if defined(WCHAR_T_IS_UTF16)
namespace base
{
typedef wchar_t char16;
typedef std::wstring string16;
typedef std::char_traits<wchar_t> string16_char_traits;
}
#elif defined(WCHAR_T_IS_UTF32)

#endif
#endif