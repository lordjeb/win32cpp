#include "pch.h"
#include "error.h"
#include "memory.h"
#include "string_extensions.h"
#include <ctype.h>

using namespace std;

wstring win32cpp::getErrorMessage(DWORD errorCode, LANGID languageId /*= LANGID_ENGLISH*/,
                                  bool allowFallback /*= true*/)
{
    auto flags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM;
    auto pString = unique_ptr<wchar_t, heap_ptr_deleter>{};
    auto nch = FormatMessageW(flags, nullptr, errorCode, languageId, (LPWSTR)&ptr_setter(pString), 0, nullptr);
    if (0 == nch)
    {
        auto lastError = GetLastError();
        if (!allowFallback)
        {
            CHECK_WIN32(lastError);
        }

        // Fallback to english if unable to get the message in the requested language
        CHECK_COUNT(
            FormatMessageW(flags, nullptr, errorCode, LANGID_ENGLISH, (LPWSTR)&ptr_setter(pString), 0, nullptr));
    }
    return trimRight(static_cast<const wchar_t*>(pString.get()), ::isspace);
}

win32cpp::check_failed::check_failed(long result, const wchar_t* message)
    : std::exception(wstr2utf8(message).c_str()), error(result), file{ nullptr }, line{ -1 }
{
}

win32cpp::check_failed::check_failed(long result, const wchar_t* file, int line, const wchar_t* message)
    : std::exception(wstr2utf8(message).c_str()), error(result), file{ file }, line{ line }
{
}

const char* win32cpp::check_failed::errorCodeMessage() const
{
    return u8"";
}

const char* win32cpp::hresult_check_failed::errorCodeMessage() const
{
    return u8"HRESULT: ";
}

const char* win32cpp::win32_check_failed::errorCodeMessage() const
{
    return u8"WCODE: ";
}

std::wstring win32cpp::check_failed::w_what() const
{
    return utf8towstr(what());
}

std::wstring getFileName(const std::wstring& path)
{
    auto slash = std::find_if(std::rbegin(path), std::rend(path), win32cpp::ispath);
    if (slash == std::rend(path))
    {
        return path;
    }

    return path.substr(std::rend(path) - slash);
}

char const* win32cpp::check_failed::what() const
{
    if (!file)
    {
        message = format("[%s0x%08x] %s", errorCodeMessage(), error, std::exception::what());
    }
    else if (line < 0)
    {
        message = format("%s: [%s0x%08x] %s", wstr2utf8(getFileName(file)).c_str(), errorCodeMessage(), error,
                         std::exception::what());
    }
    else
    {
        message = format("%s(%d): [%s0x%08x] %s", wstr2utf8(getFileName(file)).c_str(), line, errorCodeMessage(), error,
                         std::exception::what());
    }

    return message.c_str();
}
