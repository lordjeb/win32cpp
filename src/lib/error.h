#pragma once
#include "debug.h"
#include <string>
#include <Windows.h>

#define CHECK_BOOL(br) win32cpp::checkBool((br), __FILEW__, __LINE__)
#define CHECK_COUNT(cnt) win32cpp::checkCount((cnt), __FILEW__, __LINE__)
#define CHECK_HR(dwr) win32cpp::checkHr((dwr), __FILEW__, __LINE__)
#define CHECK_WIN32(dwr) win32cpp::checkWin32((dwr), __FILEW__, __LINE__)
#define CHECK_EQ(r1, r2) win32cpp::checkEq((r1), (r2), __FILEW__, __LINE__)
#define CHECK_NE(r1, r2) win32cpp::checkNe((r1), (r2), __FILEW__, __LINE__)

#define LANGID_ENGLISH 1033

namespace win32cpp
{
    struct check_failed
    {
        explicit check_failed(long result) : error(result)
        {
        }

        check_failed(long result, const std::wstring& file, int line) : error(result), file{ file }, line{ line }
        {
        }

        long error;
        std::wstring file;
        int line;
    };

    inline void checkBool(BOOL br)
    {
        if (FALSE == br)
        {
            throw check_failed(GetLastError());
        }
    }

    inline void checkBool(BOOL br, const wchar_t* file, int line)
    {
        if (FALSE == br)
        {
            auto gle = GetLastError();
            outputDebugStringEx(L"%s(%d): BOOL result failure (%d)\n", file, line, gle);
            throw check_failed(gle, file, line);
        }
    }

    inline void checkBool(bool br)
    {
        if (false == br)
        {
            throw check_failed(GetLastError());
        }
    }

    inline void checkBool(bool br, const wchar_t* file, int line)
    {
        if (false == br)
        {
            auto gle = GetLastError();
            outputDebugStringEx(L"%s(%d): bool result failure (%d)\n", file, line, gle);
            throw check_failed(gle, file, line);
        }
    }

    inline void checkCount(int c)
    {
        if (0 == c)
        {
            throw check_failed(GetLastError());
        }
    }

    inline void checkCount(int c, const wchar_t* file, int line)
    {
        if (0 == c)
        {
            auto gle = GetLastError();
            outputDebugStringEx(L"%s(%d): count result failure (%d)\n", file, line, gle);
            throw check_failed(gle, file, line);
        }
    }

    inline void checkHr(HRESULT hr)
    {
        if (FAILED(hr))
        {
            throw check_failed(hr);
        }
    }

    inline void checkHr(HRESULT hr, const wchar_t* file, int line)
    {
        if (FAILED(hr))
        {
            outputDebugStringEx(L"%s(%d): HRESULT result failure (0x%08x)\n", file, line, hr);
            throw check_failed(hr, file, line);
        }
    }

    inline void checkWin32(DWORD dwr)
    {
        if (ERROR_SUCCESS != dwr)
        {
            throw check_failed(dwr);
        }
    }

    inline void checkWin32(DWORD dwr, const wchar_t* file, int line)
    {
        if (ERROR_SUCCESS != dwr)
        {
            outputDebugStringEx(L"%s(%d): WIN32 result failure (0x%08x)\n", file, line, dwr);
            throw check_failed(dwr, file, line);
        }
    }

    template <typename T>
    inline void checkEq(T expected, T actual)
    {
        if (expected != actual)
        {
            throw check_failed(0);
        }
    }

    template <typename T>
    inline void checkEq(T expected, T actual, const wchar_t* file, int line)
    {
        if (expected != actual)
        {
            outputDebugStringEx(L"%s(%d): check failure\n", file, line);
            throw check_failed(0, file, line);
        }
    }

    template <typename T>
    inline void checkNe(T expected, T actual)
    {
        if (expected == actual)
        {
            throw check_failed(0);
        }
    }

    template <typename T>
    inline void checkNe(T expected, T actual, const wchar_t* file, int line)
    {
        if (expected == actual)
        {
            outputDebugStringEx(L"%s(%d): check failure\n", file, line);
            throw check_failed(0, file, line);
        }
    }

    std::wstring getErrorMessage(DWORD dw, LANGID languageId = LANGID_ENGLISH);
}
