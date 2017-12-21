#pragma once
#include <algorithm>
#include <string>

namespace win32cpp
{
    _When_(_Param_(1) == 0, _Post_equal_to_(0)) _Check_return_ _CRT_JIT_INTRINSIC inline int __cdecl ispath(_In_ int _C)
    {
        return _C == L'\\' || _C == L'/';
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

    inline std::wstring const& to_wstring(std::wstring const& s)
    {
        return s;
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

    // Convert a std::wstring to a std::string
    auto wstr2str(const std::wstring&) -> std::string;
}
