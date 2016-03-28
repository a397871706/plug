#ifndef _REGEDIT_H_
#define _REGEDIT_H_

#include <string>
#include <vector>
#include <stdint.h>
#include <wtypes.h>
#include "base_export.h"
#include "macro.h"

class BASE_EXPORT RegKey
{
public:
    RegKey();
    explicit RegKey(HKEY key);
    RegKey(HKEY rootkey, const wchar_t* subkey, REGSAM access);
    ~RegKey();

    LONG Create(HKEY rootkey, const wchar_t* subkey, REGSAM access);


    LONG CreateWithDisposition(HKEY rootkey, const wchar_t* subkey,
                               DWORD* disposition, REGSAM access);

    LONG CreateKey(const wchar_t* name, REGSAM access);

    LONG Open(HKEY rootkey, const wchar_t* subkey, REGSAM access);

    LONG OpenKey(const wchar_t* relative_key_name, REGSAM access);

    void Close();

    bool HasValue(const wchar_t* value_name) const;

    DWORD GetValueCount() const;

    LONG GetValueNameAt(int index, std::wstring* name) const;

    bool Valid() const { return key_ != NULL; }

    LONG DeleteKey(const wchar_t* name);

    LONG DeleteEmptyKey(const wchar_t* name);

    LONG DeleteValue(const wchar_t* name);

    LONG ReadValueDW(const wchar_t* name, DWORD* out_value) const;

    LONG ReadInt64(const wchar_t* name, int64_t* out_value) const;

    LONG ReadValue(const wchar_t* name, std::wstring* out_value) const;

    LONG ReadValues(const wchar_t* name, std::vector<std::wstring>* values);

    LONG ReadValue(const wchar_t* name,
                   void* data,
                   DWORD* dsize,
                   DWORD* dtype) const;

    LONG WriteValue(const wchar_t* name, DWORD in_value);

    LONG WriteValue(const wchar_t* name, const wchar_t* in_value);

    LONG WriteValue(const wchar_t* name,
                    const void* data,
                    DWORD dsize,
                    DWORD dtype);

    HKEY Handle() const { return key_; }
private:
    static LONG RegDeleteKeyExWrapper(HKEY hKey,
                                      const wchar_t* lpSubKey,
                                      REGSAM samDesired,
                                      DWORD Reserved);

    static LONG RegDelRecurse(HKEY root_key,
                              const std::wstring& name,
                              REGSAM access);

    HKEY key_; 
    REGSAM wow64access_;

    DISALLOW_COPY_AND_ASSIGN(RegKey);
};

#endif