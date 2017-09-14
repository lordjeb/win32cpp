# win32cpp

Writing Win32 code in straight-C can be error-prone; Sometimes even the best programmers forget to call CloseHandle in an error condition. C++ has introduced a lot of default functionality that really can assist in always writing correct code by default.

This library attempts to bridge the gap between modern C++ and Win32 programming. It offers assistance with handles that auto-close when they go out of scope, pointers that auto-free in the same conditions, as well as some easy methods for detecting and handling errors with exceptions.
