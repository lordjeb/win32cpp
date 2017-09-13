#include "stdafx.h"
#include "debug.hpp"
#include "windows_api.hpp"

using namespace std;

namespace win32cpp
{
	tracer::tracer(wchar_t const* filename, unsigned const line) : m_filename{ filename }, m_line{ line }
	{
	}

	auto tracer::operator()(wchar_t const* pFormat, ...) const -> void
	{
		va_list args;
		va_start(args, pFormat);
		auto cch1 = _scwprintf(L"%s(%d): ", m_filename, m_line);
		ASSERT(-1 != cch1);
		auto cch2 = _vscwprintf(pFormat, args);
		ASSERT(-1 != cch2);
		auto cch = cch1 + cch2 + 1;
		auto pString = make_unique<wchar_t[]>(cch);
		VERIFY(-1 != swprintf_s(pString.get(), cch, L"%s(%d): ", m_filename, m_line));
		VERIFY(-1 != _vsnwprintf_s(pString.get() + cch1, cch - cch1, _TRUNCATE, pFormat, args));
		getGlobalWindowsApi()->OutputDebugStringW(pString.get());
		va_end(args);
	}

	auto win32cpp::outputDebugStringEx(wchar_t const* pFormat, ...) -> void
	{
		va_list args;
		va_start(args, pFormat);
		auto cch = _vscwprintf(pFormat, args) + 1;
		ASSERT(-1 != cch);
		auto pString = make_unique<wchar_t[]>(cch);
		VERIFY(-1 != _vsnwprintf_s(pString.get(), cch, _TRUNCATE, pFormat, args));
		getGlobalWindowsApi()->OutputDebugStringW(pString.get());
		va_end(args);
	}
}
