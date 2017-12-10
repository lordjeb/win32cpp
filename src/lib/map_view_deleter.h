#pragma once
#include "debug.h"
#include <Windows.h>

namespace win32cpp
{
    class map_view_deleter
    {
    public:
        auto operator()(void* value) const throw() -> void
        {
            VERIFY(::UnmapViewOfFile(value));
        }
    };
}
