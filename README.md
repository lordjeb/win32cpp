# win32cpp

[![Build Status](https://ci.appveyor.com/api/projects/status/github/lordjeb/win32cpp?svg=true)](https://ci.appveyor.com/project/lordjeb/win32cpp)

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

For more detailed examples and documentation, please see the [Docs](docs/OVERVIEW.md).

## Building win32cpp

win32cpp is built with CMake. A Powershell script is included at the root of the repository that will assist in building various configurations. Prior to running the following command, make sure that cmake.exe is in the PATH.

`build.ps1 -VisualStudioVersion [All|Vs2015|Vs2017|Vs2019] -Runtime [All|Static|Dynamic] -Config [All|Debug|Release] -Platform [All|Win32|x64] -Package -VerboseBuild`
