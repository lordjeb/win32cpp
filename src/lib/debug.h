#pragma once
#include <crtdbg.h>


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
	struct tracer
	{
		wchar_t const* m_filename;
		unsigned m_line;

		tracer(wchar_t const* filename, unsigned const line);
		auto operator()(wchar_t const* pFormat, ...) const -> void;
	};

	auto outputDebugStringEx(wchar_t const* pFormat, ...) -> void;
}

#ifdef _DEBUG
#define TRACE win32cpp::tracer( __WFILE__, __LINE__ )
#else
#define TRACE __noop
#endif

#define RELTRACE win32cpp::tracer( __WFILE__, __LINE__ )
