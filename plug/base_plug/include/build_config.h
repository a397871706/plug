#ifndef _BUILD_CONFIG_H_
#define _BUILD_CONFIG_H_


#if defined(_WIN32)
#define  WCHAR_T_IS_UTF16
#endif


#if defined(_MSC_VER)
#define  COMPILER_MSVC 1
#endif

#endif