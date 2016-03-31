#include "app_reg.h"

#include <sstream>

#include "../../../base_plug/include/regedit.h"

namespace plug
{
bool plug::SetAppReg()
{
    wchar_t* softname = L"QQLinkGame";
    wchar_t* regeditKey = L"SOFTWARE";
    wchar_t* applicationName = L"ApplicationName";
    wchar_t* applicationNameText = L"QQ连连看外挂";

    RegKey key;
    std::wstringstream os;
    os << regeditKey << L"\\" << softname;
    if (ERROR_SUCCESS != key.Open(HKEY_LOCAL_MACHINE, os.str().c_str(),
        KEY_ALL_ACCESS))
    {
        if (ERROR_SUCCESS != key.Create(HKEY_LOCAL_MACHINE, os.str().c_str(),
            KEY_SET_VALUE | KEY_WOW64_32KEY))
        {
            return false;
        }
    }

    RegKey subKey;
    if (ERROR_SUCCESS != subKey.Open(key.Handle(), applicationName,
        KEY_ALL_ACCESS))
    {
        if (ERROR_SUCCESS != subKey.Create(key.Handle(), applicationName,
            KEY_ALL_ACCESS))
        {
            return false;
        }
    }

    subKey.WriteValue(applicationName, applicationNameText);
    return true;
}

bool DeleteAppReg()
{
    wchar_t* softname = L"QQLinkGame";
    wchar_t* regeditKey = L"SOFTWARE";
    std::wostringstream os;
    os << regeditKey << L"\\" << softname;
    RegKey reg(HKEY_LOCAL_MACHINE, os.str().c_str(), KEY_ALL_ACCESS);
    if (reg.Valid())
    {
        if (ERROR_SUCCESS != reg.DeleteKey(os.str().c_str()))
            return false;
    }

    return true;
}

}