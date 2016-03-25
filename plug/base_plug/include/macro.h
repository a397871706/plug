#ifndef _MACRO_H_
#define _MACRO_H_


#if defined(_BASE_EXPORT)
#define BASE_EXPORT __declspec(dllimport)
#else
#define BASE_EXPORT __declspec(dllexport)
#endif




#endif