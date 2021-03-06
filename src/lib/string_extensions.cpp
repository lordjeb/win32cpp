#include "string_extensions.h"
#include <locale>
#include <Windows.h>

using namespace std;
using namespace win32cpp;

typedef codecvt<wchar_t, char, mbstate_t> codecvt_char_wchar;

auto win32cpp::str2wstr(const string& in) -> wstring
{
    wstring_convert<codecvt_char_wchar> conv;
    return conv.from_bytes(in);
}

auto win32cpp::wstr2str(const wstring& in) -> string
{
    wstring_convert<codecvt_char_wchar> conv;
    return conv.to_bytes(in);
}

auto win32cpp::wstr2utf8(const wstring& in) -> string
{
    auto size = static_cast<int>(in.size());
    int cbNeeded = WideCharToMultiByte(CP_UTF8, 0, in.c_str(), size, nullptr, 0, nullptr, nullptr);

    string out(cbNeeded, 0);
    WideCharToMultiByte(CP_UTF8, 0, in.c_str(), size, &out[0], cbNeeded, nullptr, nullptr);

    return out;
}

auto win32cpp::utf8towstr(const string& in) -> wstring
{
    auto size = static_cast<int>(in.size());
    int cchRequired = MultiByteToWideChar(CP_UTF8, 0, in.c_str(), size, nullptr, 0);

    wstring out(cchRequired, 0);
    MultiByteToWideChar(CP_UTF8, 0, in.c_str(), size, &out[0], cchRequired);

    return out;
}
