#pragma once
#include <memory>
#include <crtdbg.h>
#include <tchar.h>
#include <Windows.h>


//
//	Wide-character versions of built-in macros
//

#define WIDEN2(x) L##x
#define WIDEN(x) WIDEN2(x)
#define __WFILE__ WIDEN(__FILE__)
#define __WFUNCTION__ WIDEN(__FUNCTION__)


//
//	Assertions
//
//	ASSERT( expression );	-- Checks the expression in debug builds, does NOTHING in release builds
//	VERIFY( expression );	-- Checks the expression in debug builds, executes but does not check the expression in release builds
//
//	Note: Recommended to use built-in C++ static_assert() for compile-time assertions
//

#define ASSERT _ASSERTE

#ifdef _DEBUG
#define VERIFY ASSERT
#else
#define VERIFY(expression) (expression)
#endif


//
//	Debug Tracing
//
//	TRACE( format-string, parameters... );					-- Output formatted string to debugger with filename and line number (debug builds only)
//	OutputDebugStringEx( format-string, parameters... );	-- Plain output to debugger (release or debug builds)
//	RELTRACE( format-string, parameters... );				-- As TRACE but includes release and debug builds
//

namespace win32cpp
{
	template <typename T>
	struct tracer
	{
		T const* m_filename;
		unsigned m_line;

		tracer(T const* filename, unsigned const line)
			: m_filename{ filename }, m_line{ line }
		{
		}
			
		auto operator()(T const* pFormat, ...) const -> void
		{
			va_list args;
			va_start(args, pFormat);
			auto cch1 = _sctprintf(_T("%s(%d): "), m_filename, m_line);
			ASSERT(-1 != cch1);
			auto cch2 = _vsctprintf(pFormat, args);
			ASSERT(-1 != cch2);
			auto cch = cch1 + cch2 + 1;
			auto pString = std::make_unique<T[]>(cch);
			VERIFY(-1 != _stprintf_s(pString.get(), cch, _T("%s(%d): "), m_filename, m_line));
			VERIFY(-1 != _vsntprintf_s(pString.get() + cch1, cch - cch1, _TRUNCATE, pFormat, args));
			OutputDebugString(pString.get());
			va_end(args);
		}
	};

	template <typename T>
	auto outputDebugStringEx(T const* pFormat, ...) -> void
	{
		va_list args;
		va_start(args, pFormat);
		auto cch = _vscwprintf(pFormat, args) + 1;
		ASSERT(-1 != cch);
		auto pString = std::make_unique<T[]>(cch);
		VERIFY(-1 != _vsnwprintf_s(pString.get(), cch, _TRUNCATE, pFormat, args));
		OutputDebugString(pString.get());
		va_end(args);
	}
}

#ifdef _DEBUG
#ifndef _UNICODE
#define TRACE win32cpp::tracer<char>( __FILE__, __LINE__ )
#else
#define TRACE win32cpp::tracer<wchar_t>( __WFILE__, __LINE__ )
#endif
#else
#define TRACE __noop
#endif

#ifndef _UNICODE
#define RELTRACE win32cpp::tracer<char>( __FILE__, __LINE__ )
#else
#define RELTRACE win32cpp::tracer<wchar_t>( __WFILE__, __LINE__ )
#endif
