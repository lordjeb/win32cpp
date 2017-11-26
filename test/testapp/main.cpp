#include "..\..\targetver.h"
#include <iostream>
#include <memory>
#include <Windows.h>
#include <Sddl.h>
#include <debug.h>
#include <error.h>
#include <module_info.h>
#include <string_extensions.h>
#include <performance.h>
#include <privilege_guard.h>
#include <memory.h>
#include <windows_constants.h>
#include <lock_guard.h>
#include <map_view_deleter.h>

using namespace std;
using namespace win32cpp;

void wmain()
{
	// debug.h
	//
	ASSERT(true);

	TRACE(L"This line will output to the debugger only in debug builds\n");

	RELTRACE(L"This line will output to the debugger in all builds\n");
	RELTRACE(L"%s\n", L"Tracing macros can be used with format specifier strings");

	outputDebugStringEx(L"This is a test\n");
}
