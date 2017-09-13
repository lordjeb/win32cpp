#include "..\targetver.h"
#include <iostream>
#include <debug.hpp>
#include <error.hpp>
#include <string_extensions.hpp>

using namespace std;
using namespace win32cpp;

void main()
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
}
