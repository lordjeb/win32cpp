#include "pch.h"
#include "debug.h"

TEST(debug_test, assertions)
{
    ASSERT(true);
}

TEST(debug_test, tracing)
{
    TRACE(L"This line will output to the debugger only in debug builds\n");

    RELTRACE(L"This line will output to the debugger in all builds\n");
    RELTRACE(L"%s\n", L"Tracing macros can be used with format specifier strings");
}

TEST(debug_test, debug_output)
{
    outputDebugStringEx(L"This is a %s\n", L"test");
}
