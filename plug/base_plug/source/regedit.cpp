#include "../include/regedit.h"

#include <assert.h>

namespace
{
const REGSAM kWow64AccessMask = KEY_WOW64_32KEY | KEY_WOW64_64KEY;
}

RegKey::RegKey()
    : key_(NULL)
    , wow64access_(0)
{

}

RegKey::RegKey(HKEY key)
    : key_(key)
    , wow64access_(0)
{

}

RegKey::RegKey(HKEY rootkey, const wchar_t* subkey, REGSAM access)
    : key_(NULL)
    , wow64access_(0)
{
    if (rootkey)
    {
        if (access & (KEY_SET_VALUE | KEY_CREATE_SUB_KEY | KEY_CREATE_LINK))
        {
            Create(rootkey, subkey, access);
        }
        else
        {
            Open(rootkey, subkey, access);
        }
    }
    else
    {
        wow64access_ = access & kWow64AccessMask;
    }
}

RegKey::~RegKey()
{

}

LONG RegKey::Create(HKEY rootkey, const wchar_t* subkey, REGSAM access)
{
    DWORD disposition_value = 0;
    return CreateWithDisposition(rootkey, subkey, &disposition_value, access);
}

LONG RegKey::CreateWithDisposition(HKEY rootkey, const wchar_t* subkey,
                                   DWORD* disposition, REGSAM access)
{
    assert(rootkey && subkey && disposition);
    HKEY subhkey = NULL;
    LONG result = ::RegCreateKeyEx(rootkey, subkey, 0, NULL,
                                   REG_OPTION_NON_VOLATILE, access, NULL, 
                                   &subhkey, disposition);
    if (ERROR_SUCCESS == result)
    {
        Close();
        key_ = subhkey;
        wow64access_ = access & kWow64AccessMask;
    }

    return result;
}

LONG RegKey::CreateKey(const wchar_t* name, REGSAM access)
{
    assert(name);
    HKEY subKey = NULL;
    LONG result = ::RegCreateKeyEx(key_, name, 0, NULL, REG_OPTION_NON_VOLATILE,
                                   access, NULL, &subKey, NULL);
    if (ERROR_SUCCESS == result)
    {
        Close();
        key_ = subKey;
        wow64access_ = access & kWow64AccessMask;
    }

    return result;
}

LONG RegKey::Open(HKEY rootkey, const wchar_t* subkey, REGSAM access)
{
    HKEY subhKey = NULL;
    LONG reslut = ::RegOpenKeyEx(rootkey, subkey, 0, access, &subhKey);
    if (ERROR_SUCCESS == reslut)
    {
        Close();
        key_ = subhKey;
        wow64access_ = access & kWow64AccessMask;
    }

    return reslut;
}

LONG RegKey::OpenKey(const wchar_t* relative_key_name, REGSAM access)
{
    assert(relative_key_name);
    if ((access & kWow64AccessMask) != wow64access_)
        return ERROR_INVALID_PARAMETER;

    HKEY subHkey = NULL;
    LONG reslut = ::RegOpenKeyEx(key_, relative_key_name, 0, access, &subHkey);
    if (reslut == ERROR_SUCCESS)
    {
        Close();
        key_ = subHkey;
        wow64access_ = access & kWow64AccessMask;
    }

    return reslut;
}

void RegKey::Close()
{
    if (key_)
    {
        ::RegCloseKey(key_);
        key_ = NULL;
        wow64access_ = 0;
    }
}

bool RegKey::HasValue(const wchar_t* value_name) const
{
    assert(value_name);
    return ::RegQueryValueEx(key_, value_name, 0, NULL, NULL, NULL) == ERROR_SUCCESS;
}

DWORD RegKey::GetValueCount() const
{
    DWORD count = 0;
    LONG result = ::RegQueryInfoKey(key_, NULL, 0, NULL, NULL, NULL, NULL,
                                    &count, NULL, NULL, NULL, NULL);
    return (result == ERROR_SUCCESS) ? count : 0;
}

LONG RegKey::GetValueNameAt(int index, std::wstring* name) const
{
    wchar_t buf[256] = { 0 };
    DWORD bufsize = sizeof(buf) / sizeof(buf[0]);
    LONG r = ::RegEnumValue(key_, index, buf, &bufsize, NULL, NULL, NULL, NULL);
    if (r == ERROR_SUCCESS)
        *name = buf;

    return r;
}

LONG RegKey::DeleteKey(const wchar_t* name)
{
    assert(key_);
    assert(name);
    HKEY subKey = NULL;

    LONG result = ::RegOpenKeyEx(key_, name, 0, READ_CONTROL | wow64access_,
                                 &subKey);
    if (result != ERROR_SUCCESS)
        return result;

    RegCloseKey(subKey);

    return RegDelRecurse(key_, std::wstring(name), wow64access_);
}

LONG RegKey::DeleteEmptyKey(const wchar_t* name)
{
    assert(key_);
    assert(name);

    HKEY target_key = NULL;
    LONG result = ::RegOpenKeyEx(key_, name, 0, KEY_READ | wow64access_,
                                 &target_key);
    if (result != ERROR_SUCCESS)
        return result;

    DWORD count = 0;
    result = ::RegQueryInfoKey(target_key, NULL, 0, NULL, NULL, NULL, NULL,
                               &count, NULL, NULL, NULL, NULL);
    ::RegCloseKey(target_key);
    if (result != ERROR_SUCCESS)
        return result;

    if (count == 0)
        return RegDeleteKeyExWrapper(key_, name, wow64access_, 0);

    return ERROR_DIR_NOT_EMPTY;
}

LONG RegKey::DeleteValue(const wchar_t* name)
{
    assert(key_);
    assert(name);    
    return ::RegDeleteValue(key_, name);
}

LONG RegKey::ReadValueDW(const wchar_t* name, DWORD* out_value) const
{
    assert(out_value);
    DWORD type = REG_DWORD;
    DWORD size = sizeof(DWORD);
    DWORD local_value = 0;
    LONG result = ReadValue(name, &local_value, &size, &type);
    if (result == ERROR_SUCCESS)
    {
        if ((type == REG_DWORD || type == REG_BINARY) && size == sizeof(DWORD))
        {
            *out_value = local_value;
        }
        else
        {
            result = ERROR_CANTREAD;
        }
    }

    return result;
}

LONG RegKey::ReadInt64(const wchar_t* name, int64_t* out_value) const
{
    assert(out_value);
    DWORD type = REG_QWORD;
    int64_t local_value = 0;
    DWORD size = sizeof(local_value);
    LONG result = ReadValue(name, &local_value, &size, &type);
    if (result == ERROR_SUCCESS)
    {
        if ((type == REG_QWORD || type == REG_BINARY) && size == sizeof(DWORD))
        {
            *out_value = local_value;
        }
        else
        {
            result = ERROR_CANTREAD;
        }
    }
    return 0;
}

LONG RegKey::ReadValue(const wchar_t* name, std::wstring* out_value) const
{
    assert(out_value);

    return 0;
}

LONG RegKey::ReadValue(const wchar_t* name, void* data, DWORD* dsize, DWORD* dtype) const
{
    return 0;
}

LONG RegKey::ReadValues(const wchar_t* name, std::vector<std::wstring>* values)
{
    return 0;
}

LONG RegKey::WriteValue(const wchar_t* name, DWORD in_value)
{
    return 0;
}

LONG RegKey::WriteValue(const wchar_t* name, const wchar_t* in_value)
{
    return 0;
}

LONG RegKey::WriteValue(const wchar_t* name, const void* data, DWORD dsize,
                        DWORD dtype)
{
    return 0;
}

LONG RegKey::RegDeleteKeyExWrapper(HKEY hKey, const wchar_t* lpSubKey,
                                   REGSAM samDesired, DWORD Reserved)
{
    typedef LSTATUS(WINAPI* RegdeleteKeyExPtr)(HKEY, LPCWSTR, REGSAM, DWORD);
    RegdeleteKeyExPtr reg_delete_key_ex_func =
        reinterpret_cast<RegdeleteKeyExPtr>(
            ::GetProcAddress(::GetModuleHandle(L"advapi32.dll"),
                             "RegDeleteKeyExW"));

    if (reg_delete_key_ex_func)
        return reg_delete_key_ex_func(hKey, lpSubKey, samDesired, Reserved);

    // Windows XP does not support RegDeleteKeyEx, so fallback to RegDeleteKey.
    return ::RegDeleteKey(hKey, lpSubKey);
}

LONG RegKey::RegDelRecurse(HKEY root_key, const std::wstring& name,
                           REGSAM access)
{
    return 0;
}
