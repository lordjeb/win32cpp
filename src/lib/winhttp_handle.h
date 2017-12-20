#pragma once
#include "debug.h"
#include "handle.h"

namespace win32cpp
{
    struct http_handle_traits
    {
        typedef HINTERNET pointer;

        static auto invalid() noexcept -> pointer
        {
            return nullptr;
        }

        static auto close(const pointer value) noexcept -> void
        {
            VERIFY(::CloseHandle(value));
        }
    };

    typedef basic_unique_handle<http_handle_traits> unique_http_handle;
}