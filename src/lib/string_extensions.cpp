#include "string_extensions.h"
#include <codecvt>

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
