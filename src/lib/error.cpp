#include "pch.h"
#include "error.h"
#include "memory.h"
#include "string_extensions.h"

using namespace std;

wstring win32cpp::getErrorMessage(DWORD errorCode, LANGID languageId /*= LANGID_ENGLISH*/)
{
    // void* pString;
    auto flags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM;
    auto x = unique_ptr<wchar_t, heap_ptr_deleter>{};
    CHECK_COUNT(FormatMessageW(flags, nullptr, errorCode, languageId, (LPWSTR)&ptr_setter(x), 0, nullptr));
    return trimRight(static_cast<const wchar_t*>(x.get()), isspace);
}

win32cpp::check_failed::check_failed(long result, const wchar_t* message) : std::exception(wstr2utf8(message).c_str()), error(result)
{
}

win32cpp::check_failed::check_failed(long result, const wchar_t* file, int line, const wchar_t* message) : std::exception(wstr2utf8(message).c_str()), error(result), file{ file }, line{ line }
{
}

std::wstring win32cpp::check_failed::w_what() const
{
	return utf8towstr(what());
}