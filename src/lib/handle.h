#pragma once
#include "debug.h"
#include "error.h"
#include <memory>
#include <vector>
#include <winhttp.h>

// using namespace std;

namespace win32cpp
{
    //
    // Handle traits. Used to specialize basic_unique_handle and basic_shared_handle for different types of handles.
    //

    struct null_handle_traits
    {
        typedef HANDLE pointer;

        static auto invalid() noexcept -> pointer
        {
            return nullptr;
        }

        static auto close(const pointer value) noexcept -> void
        {
            VERIFY(::CloseHandle(value));
        }
    };

    struct invalid_handle_traits
    {
        typedef HANDLE pointer;

        static auto invalid() noexcept -> pointer
        {
            return INVALID_HANDLE_VALUE;
        }

        static auto close(const pointer value) noexcept -> void
        {
            VERIFY(::CloseHandle(value));
        }
    };

    struct find_handle_traits
    {
        typedef HANDLE pointer;

        static auto invalid() noexcept -> pointer
        {
            return INVALID_HANDLE_VALUE;
        }

        static auto close(const pointer value) noexcept -> void
        {
            VERIFY(::FindClose(value));
        }
    };

    struct registry_handle_traits
    {
        typedef HKEY pointer;

        static auto invalid() noexcept -> pointer
        {
            return nullptr;
        }

        static auto close(const pointer value) noexcept -> void
        {
            // For usermode code, rather than list the explicit predefined keys, we will say that
            // anything with the most significant bit set should not be closed. That is what kernel
            // handles look like, so we should be okay as long as dealing with registry keys.
            if (value < HKEY_CLASSES_ROOT)
            {
                VERIFY(ERROR_SUCCESS == ::RegCloseKey(value));
            }
        }
    };

    struct service_handle_traits
    {
        typedef SC_HANDLE pointer;

        static auto invalid() noexcept -> pointer
        {
            return nullptr;
        }

        static auto close(const pointer value) noexcept -> void
        {
            VERIFY(::CloseServiceHandle(value));
        }
    };

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

    //
    // basic_unique_handle. Class template for a handle wrapper that allows only a single owner.
    //

    template <typename Traits>
    class basic_unique_handle
    {
    protected:
        typedef typename Traits::pointer pointer;

    public:
        explicit basic_unique_handle(pointer value = Traits::invalid()) noexcept : m_value{ value }
        {
        }

        virtual ~basic_unique_handle() noexcept
        {
            close();
        }

        // Copy (deleted)
        basic_unique_handle(const basic_unique_handle&) = delete;
        basic_unique_handle& operator=(const basic_unique_handle&) = delete;

        // Move
        basic_unique_handle(basic_unique_handle&& o) noexcept : m_value{ o.release() }
        {
        }

        auto operator=(basic_unique_handle&& o) noexcept -> basic_unique_handle&
        {
            if (this != &o)
            {
                reset(o.release());
            }
            return *this;
        }

        explicit operator bool() const noexcept
        {
            return Traits::invalid() != m_value;
        }

        auto get() const noexcept -> pointer
        {
            return m_value;
        }

        auto get_address_of() noexcept -> pointer*
        {
            ASSERT(!*this);
            return &m_value;
        }

        auto release() noexcept -> pointer
        {
            auto value = m_value;
            m_value = Traits::invalid();
            return value;
        }

        auto reset(pointer value = Traits::invalid()) noexcept -> bool
        {
            if (m_value != value)
            {
                close();
                m_value = value;
            }
            return static_cast<bool>(*this);
        }

        auto swap(basic_unique_handle<Traits>& other) noexcept -> void
        {
            std::swap(m_value, other.m_value);
        }

    private:
        pointer m_value;

        auto close() noexcept -> void
        {
            if (*this)
            {
                Traits::close(m_value);
            }
        }
    };

    template <typename Traits>
    auto swap(basic_unique_handle<Traits>& left, basic_unique_handle<Traits>& right) noexcept -> void
    {
        left.swap(right);
    }

    template <typename Traits>
    auto operator==(const basic_unique_handle<Traits>& left, const basic_unique_handle<Traits>& right) noexcept -> bool
    {
        return left.get() == right.get();
    }

    template <typename Traits>
    auto operator!=(const basic_unique_handle<Traits>& left, const basic_unique_handle<Traits>& right) noexcept -> bool
    {
        return left.get() != right.get();
    }

    template <typename Traits>
    auto operator>(const basic_unique_handle<Traits>& left, const basic_unique_handle<Traits>& right) noexcept -> bool
    {
        return left.get() > right.get();
    }

    template <typename Traits>
    auto operator>=(const basic_unique_handle<Traits>& left, const basic_unique_handle<Traits>& right) noexcept -> bool
    {
        return left.get() >= right.get();
    }

    template <typename Traits>
    auto operator<(const basic_unique_handle<Traits>& left, const basic_unique_handle<Traits>& right) noexcept -> bool
    {
        return left.get() < right.get();
    }

    template <typename Traits>
    auto operator<=(const basic_unique_handle<Traits>& left, const basic_unique_handle<Traits>& right) noexcept -> bool
    {
        return left.get() <= right.get();
    }

    template <typename Traits>
    auto waitable_handles(const std::vector<basic_unique_handle<Traits>>& v) noexcept
        -> const std::vector<typename Traits::pointer>
    {
        std::vector<typename Traits::pointer> r;
        for (auto& h : v)
        {
            r.emplace_back(h.get());
        }
        return r;
    }

    //
    // Typedef specializations.
    //

    typedef basic_unique_handle<null_handle_traits> unique_handle;
    typedef basic_unique_handle<null_handle_traits> unique_mapping_handle;
    typedef basic_unique_handle<invalid_handle_traits> unique_file_handle;
    typedef basic_unique_handle<find_handle_traits> unique_find_handle;
    typedef basic_unique_handle<registry_handle_traits> unique_registry_handle;
    typedef basic_unique_handle<service_handle_traits> unique_service_handle;
    typedef basic_unique_handle<http_handle_traits> unique_http_handle;

    class unique_token_handle : public basic_unique_handle<null_handle_traits>
    {
        bool m_impersonating;

    public:
        explicit unique_token_handle(
            basic_unique_handle<null_handle_traits>::pointer value = null_handle_traits::invalid(),
            bool impersonating = false) noexcept
            : basic_unique_handle<null_handle_traits>(value)
        {
        }

        virtual ~unique_token_handle() noexcept
        {
            if (m_impersonating)
            {
                CHECK_BOOL(RevertToSelf());
            }
        }

        // Copy (deleted)
        unique_token_handle(const unique_token_handle&) = delete;
        unique_token_handle& operator=(const unique_token_handle&) = delete;

        // Move
        unique_token_handle(unique_token_handle&& o) noexcept : m_impersonating{ o.m_impersonating }
        {
        }

        auto operator=(unique_token_handle&& o) noexcept -> basic_unique_handle&
        {
            if (this != &o)
            {
                m_impersonating = o.m_impersonating;

                o.m_impersonating = false;
            }
            return *this;
        }
    };
}
