# win32cpp

Writing Win32 code in straight-C can be error-prone; Sometimes even the best programmers forget to call CloseHandle in an error condition. C++ has introduced a lot of default functionality that really can assist in always writing correct code by default.

This library attempts to bridge the gap between modern C++ and Win32 programming. It offers assistance with handles that auto-close when they go out of scope, pointers that auto-free in the same conditions, as well as some easy methods for detecting and handling errors with exceptions.

What it does not intend to be is a heavy wrapper around Win32 functionality. There are plenty of frameworks out there that try to wrap everything, but that's not what win32cpp is about. win32cpp creates a thin layer of helper functionality that lets you still write Win32 code that looks basically like Win32 code.

For example, instead of opening a file handle like this...

```c++
HANDLE my_handle = CreateFile(L"filename", ...);
if (INVALID_HANDLE_VALUE != my_handle)
{
    ReadFile(my_handle, ...);
    CloseHandle(my_handle);
}
```

You would do this...

```c++
auto my_handle = unique_file_handle{ CreateFile(L"filename", ...) };
if (my_handle)
{
    ReadFile(my_handle.get(), ...);
}
```

If you wanted to get the temporary path and add a filename to it, instead of the following...

```c++
WCHAR tempPathBuffer[MAX_PATH];
if (0 == GetTempPathW(MAX_PATH, tempPathBuffer))
{
    wcscat(tempPathBuffer, L"filename.tmp");
}
```

You might try this...

```c++
appendPath(getTempPath(), L"filename.tmp");
```
