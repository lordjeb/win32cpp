# Overview of win32cpp

## Contents

1. [Debugging and Tracing](#debugging-and-tracing)
1. [Error Handling](#error-handling)
1. [RAII-style Handles](#raii-style-handles)

## Debugging and Tracing

win32cpp offers a number of methods for tracing your code exection and writing formatted output to an attached debugger. It also provides addition options for using ASSERTions that still execute in Release code.

`#include <win32cpp\debug.h>`

`VERIFY(condition)` - Executes the conditional code in Release code, but calls it with _ASSERTE in Debug code.

`TRACE(format_string, args...)` - Writes formatted output with the source file and line number to the debugger in Debug code.

`RELTRACE(format_string, args...)` - Writes formatted output with the source file and line number to the debugger in Release and Debug code.

`outputDebugStringEx(format_string, args...)` - Writes formatted output to the debugger in Release and Debug code.

## Error Handling

win32cpp offers a standard way of checking and handling Win32 error codes. A set of macros is provided for checking the result of Win32 functions and throwing a `check_failed` exception if the result is an error. The exception can contain the GetLastError() value as well as the source file and line number of the exception.

`#include <win32cpp\error.h>`

Macro | Check performed
----- | ---------------
CHECK_BOOL | Throws exception if the value is FALSE or false
CHECK_COUNT | Throws exception if the value is zero
CHECK_HR | Throws exception if FAILED() is true
CHECK_WIN32 | Throws exception if the value is not ERROR_SUCCESS
CHECK_EQ | Throws exception if the two provided values are not equal

This can result in greatly-improved readability of Win32 code:

```c++
CHECK_BOOL(ReadFile(my_handle.get(), ...));
```

Also included is a function `getErrorMessage` to return a displayable error message.

## RAII-style Handles

Handles are used all throughout Windows code, and a common problem is leaked handles. The generic `basic_unique_handle` type provides an RAII-style wrapper for all kinds of handles. It can be specialized to handle the various values for invalid handles, and for the various functions that should be used to close the handles.

`#include <win32cpp\handle.h>`

For example, here is how the specialization for file handles would be used to open a file and read data from it:

```c++
auto my_handle = unique_file_handle{ CreateFile(L"filename", ...) };
if (my_handle)
{
    ReadFile(my_handle.get(), ...);
}
```

Here is a table showing the specializations currently implemented:

Type | Wrapped type | Invalid value | Close function
---- | ------------ | ------------- | --------------
unique_handle | HANDLE | nullptr | CloseHandle()
unique_mapping_handle | HANDLE | nullptr | CloseHandle()
unique_file_handle | HANDLE | INVALID_HANDLE_VALUE | CloseHandle()
unique_find_handle | HANDLE | INVALID_HANDLE_VALUE | FindClose()
unique_registry_handle | HKEY | nullptr | RegCloseKey()
unique_library_handle | HMODULE | nullptr | FreeLibrary()
unique_service_handle | SC_HANDLE | nullptr | CloseServiceHandle()
unique_http_handle | HINTERNET | nullptr | CloseHandle()

There is also an extended wrapper for the type of token that adds tracking a token being used for impersonation and using it to revert the impersonation when the object goes out of scope.
