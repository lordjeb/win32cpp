#include "..\targetver.h"
#include <iostream>
#include <Windows.h>
#include <Sddl.h>
#include <debug.h>
#include <error.h>
#include <module_info.h>
#include <string_extensions.h>
#include <performance.h>
#include <privilege_guard.h>
#include <ptr_deleter.h>
#include <ptr_setter.h>
#include <windows_constants.h>
#include <lock_guard.h>
#include <map_view_deleter.h>

using namespace std;
using namespace win32cpp;

void test_file_mapping()
{
	auto file = unique_file_handle{ CreateFile(L"test", GENERIC_ALL, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_DELETE_ON_CLOSE, NULL) };
	
	auto file_mapping = unique_ptr<IFileMapping>{ new FileMappingImpl{} };
	auto mapping = unique_mapping_handle{ file_mapping->CreateFileMapping(file.get(), NULL, PAGE_READWRITE, 0, 4096, NULL) };
	if (mapping)
	{
		auto view = unique_ptr<void, map_view_deleter>{ file_mapping->MapViewOfFile(mapping.get(), FILE_MAP_ALL_ACCESS, 0, 0, 4096), map_view_deleter{ file_mapping.get() } };
	}
}

void wmain()
{
	// debug.h
	//
	ASSERT(true);

	TRACE(L"This line will output to the debugger only in debug builds\n");

	RELTRACE(L"This line will output to the debugger in all builds\n");
	RELTRACE(L"%s\n", L"Tracing macros can be used with format specifier strings");

	outputDebugStringEx(L"This is a test\n");

	// error.h
	//
	CHECK_HR(S_OK);
	CHECK_BOOL(true);
	CHECK_BOOL(TRUE);
	CHECK_WIN32(ERROR_SUCCESS);
	CHECK(ERROR_SUCCESS, ERROR_SUCCESS);

	wcout << getErrorMessage(ERROR_FILE_NOT_FOUND) << endl;

	// string_extensions.h
	//
	cout << "(" << trim(std::string{ "  text  " }) << ")" << endl;
	wcout << L"(" << trim(std::wstring{ L"  text  " }) << L")" << endl;
	cout << "(" << trimLeft(std::string{ "  text  " }) << ")" << endl;
	wcout << L"(" << trimLeft(std::wstring{ L"  text  " }) << L")" << endl;
	cout << "(" << trimRight(std::string{ "  text  " }) << ")" << endl;
	wcout << L"(" << trimRight(std::wstring{ L"  text  " }) << L")" << endl;
	wcout << L"(" << trimPath(std::wstring{ LR"(\directory\)" }) << L")" << endl;

	wcout << L"(" << appendPath(L"C:", L"directory", L"filename.txt") << L")" << endl;

	// module_info.h
	//
	wcout << getModulePath() << endl;
	wcout << getModuleFilename() << endl;
	wcout << getTempPath() << endl;

	// performance.h
	//
	wcout << measureElapsedTime([]() {Sleep(10); }) << endl;

	// privilege_guard.h
	//
	auto threadToken = getThreadToken();
	auto shutdown_privilege = privilege_guard{ threadToken.get(), L"SeShutdownPrivilege" }; // Disabled by default
	auto change_notify_privilege = privilege_guard{ threadToken.get(), L"SeChangeNotifyPrivilege" }; // Enabled by default
	wcout << L"Acquired Shutdown and ChangeNotify privileges" << endl;

	// ptr_deleter.h
	//
	wcout << L"Calling memory allocation routines" << endl;
	auto hptr = heap_ptr{ HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 256) };
	auto lptr = local_ptr{ LocalAlloc(LPTR, 256) };
	auto gptr = global_ptr{ GlobalAlloc(GPTR, 256) };

	auto pSid = PSID{};
	auto sidIdentifierAuthority = SID_IDENTIFIER_AUTHORITY{ SECURITY_NT_AUTHORITY };
	CHECK_BOOL(AllocateAndInitializeSid(&sidIdentifierAuthority, 1, DOMAIN_USER_RID_ADMIN, 0, 0, 0, 0, 0, 0, 0, &pSid));
	auto sid = sid_ptr{ pSid };

	auto pStringSid = LPTSTR{};
	CHECK_BOOL(ConvertSidToStringSid(sid.get(), &pStringSid));
	auto stringSid = local_ptr{ pStringSid };
	wcout << L"Sid: " << (LPTSTR)stringSid.get() << endl;

	// ptr_setter.h
	//
	auto sid2 = sid_ptr{};
	CHECK_BOOL(AllocateAndInitializeSid(&sidIdentifierAuthority, 1, DOMAIN_USER_RID_ADMIN, 0, 0, 0, 0, 0, 0, 0, &ptr_setter(sid2)));

	auto stringSid2 = local_ptr{};
	CHECK_BOOL(ConvertSidToStringSid(sid2.get(), (LPTSTR*)&ptr_setter(stringSid2)));
	wcout << L"Sid: " << (LPTSTR)stringSid2.get() << endl;

	// handle.h
	//
	WIN32_FIND_DATA find_data;
	auto find_handle = unique_find_handle{ FindFirstFile(LR"(C:\*)", &find_data) };

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

	test_file_mapping();
}
