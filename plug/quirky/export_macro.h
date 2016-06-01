#ifndef _EXPORT_MACRO_H_
#define _EXPORT_MACRO_H_

#if !defined(DLL_EXOPRT)
#define  EXPORT __declspec(dllexport)
#else
#define  EXPORT __declspec(dllimport)
#endif

#endif