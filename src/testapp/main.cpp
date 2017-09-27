#include "..\targetver.h"
#include <iostream>
#include <Windows.h>
#include <Sddl.h>
#include <debug.hpp>
#include <error.hpp>
#include <module_info.hpp>
#include <string_extensions.hpp>
#include <performance.hpp>
#include <privilege_guard.hpp>
#include <ptr_deleter.hpp>
#include <ptr_setter.hpp>

using namespace std;
using namespace win32cpp;

void wmain()
{
    // debug.hpp
    //
    ASSERT(true);
    
    TRACE(L"This line will output to the debugger only in debug builds\n");

    RELTRACE(L"This line will output to the debugger in all builds\n");
    RELTRACE(L"%s\n", L"Tracing macros can be used with format specifier strings");

	outputDebugStringEx(L"This is a test\n");

    // error.hpp
    //
	CHECK_HR(S_OK);
	CHECK_BOOL(true);
	CHECK_BOOL(TRUE);
    CHECK_WIN32(ERROR_SUCCESS);
    CHECK(ERROR_SUCCESS, ERROR_SUCCESS);

    wcout << getErrorMessage(ERROR_FILE_NOT_FOUND) << endl;

    // string_extensions.hpp
    //
    cout << "(" << trim(std::string{"  text  "}) << ")" << endl;
    wcout << L"(" << trim(std::wstring{L"  text  "}) << L")" << endl;
    cout << "(" << trimLeft(std::string{"  text  "}) << ")" << endl;
    wcout << L"(" << trimLeft(std::wstring{L"  text  "}) << L")" << endl;
    cout << "(" << trimRight(std::string{"  text  "}) << ")" << endl;
    wcout << L"(" << trimRight(std::wstring{L"  text  "}) << L")" << endl;
    wcout << L"(" << trimPath(std::wstring{LR"(\directory\)"}) << L")" << endl;

    wcout << L"(" << appendPath(L"C:", L"directory", L"filename.txt") << L")" << endl;

    // module_info.hpp
    //
    wcout << getModulePath() << endl;
    wcout << getModuleFilename() << endl;
    wcout << getTempPath() << endl;
    
    // performance.hpp
    //
    wcout << measureElapsedTime([](){Sleep(10);}) << endl;

    // privilege_guard.hpp
    //
    auto threadToken = getThreadToken();
    auto shutdown_privilege = privilege_guard{ threadToken.get(), L"SeShutdownPrivilege" }; // Disabled by default
    auto change_notify_privilege = privilege_guard{ threadToken.get(), L"SeChangeNotifyPrivilege" }; // Enabled by default
    wcout << L"Acquired Shutdown and ChangeNotify privileges" << endl;

    // ptr_deleter.hpp
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

    // ptr_setter.hpp
    //
    auto sid2 = sid_ptr{};
    CHECK_BOOL(AllocateAndInitializeSid(&sidIdentifierAuthority, 1, DOMAIN_USER_RID_ADMIN, 0, 0, 0, 0, 0, 0, 0, &ptr_setter(sid2)));

    auto stringSid2 = local_ptr{};
    CHECK_BOOL(ConvertSidToStringSid(sid2.get(), (LPTSTR*)&ptr_setter(stringSid2)));
    wcout << L"Sid: " << (LPTSTR)stringSid2.get() << endl;
}
