#pragma once
#include <crtdbg.h>

// Wide-character versions of built-in macros
#define WIDEN2(x) L##x
#define WIDEN(x) WIDEN2(x)
#define __WFILE__ WIDEN(__FILE__)
#define __WFUNCTION__ WIDEN(__FUNCTION__)

// Checks the expression in debug builds, does NOTHING in release builds
#define ASSERT _ASSERTE

// Checks the expression in debug builds, executes but does not check the expression in release builds
#ifdef _DEBUG
#define VERIFY ASSERT
#else
#define VERIFY(expression) (expression)
#endif

namespace win32cpp
{
    struct tracer
    {
        wchar_t const* m_filename;
        unsigned m_line;

        tracer(wchar_t const* filename, unsigned const line);
        auto operator()(wchar_t const* pFormat, ...) const -> void;
    };

    // Plain output to debugger (release or debug builds)
    auto outputDebugStringEx(wchar_t const* pFormat, ...) -> void;
}

// Output formatted string to debugger with filename and line  number (debug builds only)
#ifdef _DEBUG
#define TRACE win32cpp::tracer(__WFILE__, __LINE__)
#else
#define TRACE __noop
#endif

// As TRACE but includes release and debug builds
#define RELTRACE win32cpp::tracer(__WFILE__, __LINE__)
