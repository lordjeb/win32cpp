#include "stdafx.h"
#include "string_extensions.hpp"
#include <codecvt>

using namespace std;
using namespace vxshared;

typedef codecvt<wchar_t, char, mbstate_t> codecvt_char_wchar;

auto vxshared::str2wstr(const string& in) -> wstring
{
	wstring_convert<codecvt_char_wchar> conv;
	return conv.from_bytes(in);
}

auto vxshared::wstr2str(const wstring& in) -> string
{
	wstring_convert<codecvt_char_wchar> conv;
	return conv.to_bytes(in);
}
