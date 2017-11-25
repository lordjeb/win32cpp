#pragma once
#include "debug.h"
#include "error.h"
#include "win32api_abstractions.h"
#include <memory>
#include <vector>
#include <winhttp.h>

//using namespace std;

namespace win32cpp
{
	//
	//	Handle traits. Used to specialize basic_unique_handle and basic_shared_handle for different types of handles.
	//

	struct null_handle_traits
	{
		typedef HANDLE pointer;

		static auto invalid() noexcept -> pointer
		{
			return nullptr;
		}
	};

	struct invalid_handle_traits
	{
		typedef HANDLE pointer;

		static auto invalid() noexcept -> pointer
		{
			return INVALID_HANDLE_VALUE;
		}
	};

	struct find_handle_traits
	{
		typedef HANDLE pointer;

		static auto invalid() noexcept -> pointer
		{
			return INVALID_HANDLE_VALUE;
		}
	};

	struct registry_handle_traits
	{
		typedef HKEY pointer;

		static auto invalid() noexcept -> pointer
		{
			return nullptr;
		}
	};

	struct service_handle_traits
	{
		typedef SC_HANDLE pointer;

		static auto invalid() noexcept -> pointer
		{
			return nullptr;
		}
	};

	struct http_handle_traits
	{
		typedef HINTERNET pointer;

		static auto invalid() noexcept -> pointer
		{
			return nullptr;
		}
	};

	template <typename Traits>
	class handle_closer
	{
	private:
		std::shared_ptr<IFile> m_pFile;

	protected:
		typedef typename Traits::pointer pointer;

	public:
		handle_closer(std::shared_ptr<IFile> pFile) : m_pFile{ pFile }
		{
		}

		auto operator()(pointer value) -> void
		{
			VERIFY(m_pFile->CloseHandle(value));
		}
	};

	//
	//	basic_unique_handle. Class template for a handle wrapper that allows only a single owner.
	//

	template <typename Traits, typename Closer>
	class basic_unique_handle
	{
	protected:
		typedef typename Traits::pointer pointer;
		typedef typename Closer closer;

	public:
		explicit basic_unique_handle(const closer& closer, pointer value = Traits::invalid()) noexcept
			: m_value { value }, m_closer{ closer }
		{
		}

		virtual ~basic_unique_handle() noexcept
		{
			close();
		}

		//	Copy (deleted)
		basic_unique_handle(const basic_unique_handle&) = delete;
		basic_unique_handle& operator=(const basic_unique_handle&) = delete;

		//	Move
		basic_unique_handle(basic_unique_handle&& o) noexcept : m_value { o.release() }
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

		auto get_closer() -> Closer
		{
			return m_closer;
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

		auto swap(basic_unique_handle<Traits, Closer>& other) noexcept -> void
		{
			std::swap(m_value, other.m_value);
		}

	private:
		pointer m_value;
		closer m_closer;

		auto close() noexcept -> void
		{
			if (*this)
			{
				m_closer(m_value);
			}
		}
	};

	template <typename Traits, typename Closer>
	auto swap(basic_unique_handle<Traits, Closer>& left, basic_unique_handle<Traits, Closer>& right) noexcept -> void
	{
		left.swap(right);
	}

	template <typename Traits, typename Closer>
	auto operator==(const basic_unique_handle<Traits, Closer>& left, const basic_unique_handle<Traits, Closer>& right) noexcept -> bool
	{
		return left.get() == right.get();
	}

	template <typename Traits, typename Closer>
	auto operator!=(const basic_unique_handle<Traits, Closer>& left, const basic_unique_handle<Traits, Closer>& right) noexcept -> bool
	{
		return left.get() != right.get();
	}

	template <typename Traits, typename Closer>
	auto operator>(const basic_unique_handle<Traits, Closer>& left, const basic_unique_handle<Traits, Closer>& right) noexcept -> bool
	{
		return left.get() > right.get();
	}

	template <typename Traits, typename Closer>
	auto operator>=(const basic_unique_handle<Traits, Closer>& left, const basic_unique_handle<Traits, Closer>& right) noexcept -> bool
	{
		return left.get() >= right.get();
	}

	template <typename Traits, typename Closer>
	auto operator<(const basic_unique_handle<Traits, Closer>& left, const basic_unique_handle<Traits, Closer>& right) noexcept -> bool
	{
		return left.get() < right.get();
	}

	template <typename Traits, typename Closer>
	auto operator<=(const basic_unique_handle<Traits, Closer>& left, const basic_unique_handle<Traits, Closer>& right) noexcept -> bool
	{
		return left.get() <= right.get();
	}

	template <typename Traits, typename Closer>
	auto waitable_handles(const std::vector<basic_unique_handle<Traits, Closer>>& v) noexcept -> const std::vector<typename Traits::pointer>
	{
		std::vector<typename Traits::pointer> r;
		for (auto& h : v)
		{
			r.emplace_back(h.get());
		}
		return r;
	}

	//
	//	Typedef specializations.
	//

	typedef basic_unique_handle<null_handle_traits, handle_closer<null_handle_traits>> unique_handle;
	typedef basic_unique_handle<null_handle_traits, handle_closer<invalid_handle_traits>> unique_mapping_handle;
	typedef basic_unique_handle<invalid_handle_traits, handle_closer<invalid_handle_traits>> unique_file_handle;
	typedef basic_unique_handle<find_handle_traits, handle_closer<find_handle_traits>> unique_find_handle;
	typedef basic_unique_handle<registry_handle_traits, handle_closer<registry_handle_traits>> unique_registry_handle;
	typedef basic_unique_handle<service_handle_traits, handle_closer<service_handle_traits>> unique_service_handle;
	typedef basic_unique_handle<http_handle_traits, handle_closer<http_handle_traits>> unique_http_handle;

	class unique_token_handle : public basic_unique_handle<null_handle_traits, handle_closer<null_handle_traits>>
	{
		bool m_impersonating;

	public:
		explicit unique_token_handle(const basic_unique_handle<null_handle_traits, handle_closer<null_handle_traits>>::closer& closer, basic_unique_handle<null_handle_traits, handle_closer<null_handle_traits>>::pointer value = null_handle_traits::invalid(), bool impersonating = false) noexcept
			: basic_unique_handle<null_handle_traits, handle_closer<null_handle_traits>>(closer, value)
		{
		}

		virtual ~unique_token_handle() noexcept
		{
			if (m_impersonating)
			{
				CHECK_BOOL(RevertToSelf());
			}
		}

		//	Copy (deleted)
		unique_token_handle(const unique_token_handle&) = delete;
		unique_token_handle& operator=(const unique_token_handle&) = delete;

		//	Move
		unique_token_handle(unique_token_handle&& o) noexcept
			: m_impersonating{ o.m_impersonating }
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
