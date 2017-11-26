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

	// privilege_guard.h
	//
	auto threadToken = getThreadToken();
	auto shutdown_privilege = privilege_guard{ threadToken.get(), L"SeShutdownPrivilege" }; // Disabled by default
	auto change_notify_privilege = privilege_guard{ threadToken.get(), L"SeChangeNotifyPrivilege" }; // Enabled by default
	wcout << L"Acquired Shutdown and ChangeNotify privileges" << endl;

	// lock_guard.h
	//
	{
		auto mutex = unique_handle{ CreateMutex(NoSecurityAttributes, NotInitiallyOwned, L"MutexName") };
		CHECK_BOOL(bool(mutex));
		{
			auto mutex_guard = mutex_lock_guard{ mutex.get() };

			// ... do something with the locked resource
		}

		// If the mutex is created as initially owned then mutex_lock_guard can be initialized to skip the locking
		auto mutex_reference = unique_handle{ CreateMutex(NoSecurityAttributes, InitiallyOwned, L"MutexName") };
		auto createMutexResult = GetLastError();
		CHECK_BOOL(bool(mutex_reference));
		{
			auto mutex_guard = mutex_lock_guard{ mutex_reference.get(), INFINITE, ERROR_ALREADY_EXISTS != createMutexResult };

			// ... do something with the locked resource
		}
	}

	{
		auto semaphore = unique_handle{ CreateSemaphore(NoSecurityAttributes, 1, 1, Unnamed) };
		CHECK_BOOL(bool(semaphore));
		{
			auto semaphore_guard = semaphore_lock_guard{ semaphore.get() };

			// ... do something with the locked resource
		}
	}
}
