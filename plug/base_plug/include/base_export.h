#ifndef _BASE_EXPORT_H_
#define _BASE_EXPORT_H_

#if defined(_BASE_EXPORT)
#define BASE_EXPORT __declspec(dllimport)
#else
#define BASE_EXPORT __declspec(dllexport)
#endif


#endif