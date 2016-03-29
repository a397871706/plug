#ifndef _SYSTEM_INFO_H_
#define _SYSTEM_INFO_H_

#include <wtypes.h>

namespace plug
{
enum Regedit
{
    ROOT = 0,
    CURRENT_USER,
    LOCAL,
    USER,
    CONFIG,
};

HKEY GetHKEY(Regedit regedit);

bool CreateRegedit();

bool DeleteRegedit();

bool ReadValue();

bool WriteValue();

}

#endif