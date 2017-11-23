#include "error.h"
#include "ptr_deleter.h"
#include "string_extensions.h"

using namespace std;

wstring win32cpp::getErrorMessage(DWORD errorCode, LANGID languageId /*= LANGID_ENGLISH*/)
{
	void* pString;
	auto flags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM;
	CHECK_COUNT(FormatMessageW(flags, nullptr, errorCode, languageId, LPWSTR(&pString), 0, nullptr));
	auto x = heap_ptr{ pString };
	return trimRight(wstring{ static_cast<wchar_t*>(x.get()) });
}
