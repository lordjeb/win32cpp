#pragma once
#include "debug.h"
#include <stdexcept>
#include <string>
#include <Windows.h>

#define CHECK_BOOL(br, ...) win32cpp::checkBool((br), __FILEW__, __LINE__, __VA_ARGS__)
#define CHECK_COUNT(cnt, ...) win32cpp::checkCount((cnt), __FILEW__, __LINE__, __VA_ARGS__)
#define CHECK_HR(dwr, ...) win32cpp::checkHr((dwr), __FILEW__, __LINE__, __VA_ARGS__)
#define CHECK_WIN32(dwr, ...) win32cpp::checkWin32((dwr), __FILEW__, __LINE__, __VA_ARGS__)
#define CHECK_EQ(r1, r2, ...) win32cpp::checkEq((r1), (r2), __FILEW__, __LINE__, __VA_ARGS__)
#define CHECK_NE(r1, r2, ...) win32cpp::checkNe((r1), (r2), __FILEW__, __LINE__, __VA_ARGS__)

#define LANGID_ENGLISH 1033

namespace win32cpp
{
    struct check_failed : public std::exception
    {
        explicit check_failed(long result, const wchar_t* message = L"");
        check_failed(long result, const wchar_t* file, int line, const wchar_t* message = L"");

        virtual ~check_failed()
        {
        }

        virtual std::wstring w_what() const;
        virtual char const* what() const override;

        long error;
        const wchar_t* file;
        int line;

    protected:
        virtual const char* errorCodeMessage() const;
        mutable std::string message;
    };

    struct hresult_check_failed : public check_failed
    {
        explicit hresult_check_failed(HRESULT result, const wchar_t* message = L"") : check_failed(result, message)
        {
        }

        hresult_check_failed(HRESULT result, const wchar_t* file, int line, const wchar_t* message = L"")
            : check_failed(result, file, line, message)
        {
        }

        virtual ~hresult_check_failed()
        {
        }

        HRESULT hresult() const
        {
            return static_cast<HRESULT>(error);
        }

    protected:
        virtual const char* errorCodeMessage() const override;
    };

    struct win32_check_failed : public check_failed
    {
        explicit win32_check_failed(DWORD result, const wchar_t* message = L"") : check_failed(result, message)
        {
        }

        win32_check_failed(DWORD result, const wchar_t* file, int line, const wchar_t* message = L"")
            : check_failed(result, file, line, message)
        {
        }

        virtual ~win32_check_failed()
        {
        }

        DWORD wcode() const
        {
            return static_cast<DWORD>(error);
        }

    protected:
        virtual const char* errorCodeMessage() const override;
    };

    inline void checkBool(bool br)
    {
        if (false == br)
        {
            throw win32_check_failed(GetLastError());
        }
    }

    inline void checkBool(bool br, const wchar_t* file, int line, const wchar_t* message = L"")
    {
        if (false == br)
        {
            auto gle = GetLastError();
            outputDebugStringEx(L"%s(%d): bool result failure (%d)\n", file, line, gle);
            throw win32_check_failed(gle, file, line, message);
        }
    }

    inline void checkBool(BOOL br)
    {
        checkBool(br == TRUE);
    }

    inline void checkBool(BOOL br, const wchar_t* file, int line, const wchar_t* message = L"")
    {
        checkBool(br == TRUE, file, line, message);
    }

    inline void checkCount(int c)
    {
        if (0 == c)
        {
            throw win32_check_failed(GetLastError());
        }
    }

    inline void checkCount(int c, const wchar_t* file, int line, const wchar_t* message = L"")
    {
        if (0 == c)
        {
            auto gle = GetLastError();
            outputDebugStringEx(L"%s(%d): count result failure (%d)\n", file, line, gle);
            throw win32_check_failed(gle, file, line, message);
        }
    }

    inline void checkHr(HRESULT hr)
    {
        if (FAILED(hr))
        {
            throw hresult_check_failed(hr);
        }
    }

    inline void checkHr(HRESULT hr, const wchar_t* file, int line, const wchar_t* message = L"")
    {
        if (FAILED(hr))
        {
            outputDebugStringEx(L"%s(%d): HRESULT result failure (0x%08x)\n", file, line, hr);
            throw hresult_check_failed(hr, file, line, message);
        }
    }

    inline void checkWin32(DWORD dwr)
    {
        if (ERROR_SUCCESS != dwr)
        {
            throw win32_check_failed(dwr);
        }
    }

    inline void checkWin32(DWORD dwr, const wchar_t* file, int line, const wchar_t* message = L"")
    {
        if (ERROR_SUCCESS != dwr)
        {
            outputDebugStringEx(L"%s(%d): WIN32 result failure (0x%08x)\n", file, line, dwr);
            throw win32_check_failed(dwr, file, line, message);
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
    inline void checkEq(T expected, T actual, const wchar_t* file, int line, const wchar_t* message = L"")
    {
        if (expected != actual)
        {
            outputDebugStringEx(L"%s(%d): check failure\n", file, line);
            throw check_failed(0, file, line, message);
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
    inline void checkNe(T expected, T actual, const wchar_t* file, int line, const wchar_t* message = L"")
    {
        if (expected == actual)
        {
            outputDebugStringEx(L"%s(%d): check failure\n", file, line);
            throw check_failed(0, file, line, message);
        }
    }

    std::wstring getErrorMessage(DWORD dw, LANGID languageId = LANGID_ENGLISH, bool allowFallback = true);
}
