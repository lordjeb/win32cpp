#pragma once
#include <algorithm>
#include <string>
#include <iostream>

namespace win32cpp
{
    _When_(_Param_(1) == 0, _Post_equal_to_(0)) _Check_return_ _CRT_JIT_INTRINSIC inline int __cdecl ispath(_In_ int _C)
    {
        return _C == L'\\' || _C == L'/';
    }

    // Trim whitespace characters from the beginning and end of a string
    template <typename _Px>
    auto trim(const std::wstring& s, _Px pred) -> std::wstring
    {
        auto front = std::find_if_not(begin(s), end(s), pred);
        auto back = std::find_if_not(rbegin(s), rend(s), pred);
        return std::wstring{ front, back.base() };
    }

    // Trim whitespace characters from the beginning of a string
    template <typename _Px>
    auto trimLeft(const std::wstring& s, _Px pred) -> std::wstring
    {
        auto front = std::find_if_not(begin(s), end(s), pred);
        return std::wstring{ front, end(s) };
    }

    // Trim whitespace from the end of a string
    template <typename _Px>
    auto trimRight(const std::wstring& s, _Px pred) -> std::wstring
    {
        auto back = std::find_if_not(rbegin(s), rend(s), pred);
        return std::wstring{ begin(s), back.base() };
    }

    inline std::wstring const& to_wstring(std::wstring const& s)
    {
        return s;
    }

    // Appends two sections of path together with a backslash separator, ensuring that
    // there are no extra separators
    template <typename... Args>
    std::wstring appendPath(Args const&... args)
    {
        std::wstring result;
        int unpack[]{ 0, (result +=
                          result.empty() ? trimRight(to_wstring(args), ispath) : L'\\' + trim(to_wstring(args), ispath),
                          0)... };
        return trimRight(result, ispath);
    }

    // Convert a std::string to a std::wstring
    auto str2wstr(const std::string&) -> std::wstring;

    // Convert a std::wstring to a std::string
    auto wstr2str(const std::wstring&) -> std::string;

    // Convert from UTF-16 to UTF-8
    auto wstr2utf8(const std::wstring& in) -> std::string;

    // Convert from UTF-8 to UTF-16
    auto utf8towstr(const std::string& in) -> std::wstring;

    // https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf/26221725#26221725 (CC0 1.0
    // licensed)
    template <typename... Args>
    auto format(const std::string& format, Args... args) -> std::string
    {
        size_t size = std::snprintf(nullptr, 0, format.c_str(), args...) + 1;   // Extra space for '\0'
        std::unique_ptr<char[]> buf(new char[size]);
        if (std::snprintf(buf.get(), size, format.c_str(), args...) < 0)
        {
            throw std::exception("Failed to format string.");
        }
        return std::string(buf.get(), buf.get() + size - 1);   // We don't want the '\0' inside
    }

    template <typename... Args>
    auto format(const std::wstring& format, Args... args) -> std::wstring
    {
#if (_MSC_VER < 1910)
// warning C6387: '_Param_(1)' could be '0':  this does not adhere to the specification for the function 'swprintf'
// It is acceptable for the buffer count to be 0 because the buffer is null
#pragma warning(suppress : 6387)
#endif
        size_t size = std::swprintf(nullptr, 0, format.c_str(), args...) + 1;   // Extra space for '\0'
        std::unique_ptr<wchar_t[]> buf(new wchar_t[size]);
        if (std::swprintf(buf.get(), size, format.c_str(), args...) < 0)
        {
            throw std::exception("Failed to format string.");
        }
        return std::wstring(buf.get(), buf.get() + size - 1);   // We don't want the '\0' inside
    }
}
