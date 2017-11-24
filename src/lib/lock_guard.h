#pragma once
#include "debug.h"
#include "error.h"
#include <windows.h>

namespace win32cpp
{
	struct mutex_lock_guard_traits
	{
		typedef HANDLE pointer;

		static auto lock(pointer value, unsigned long timeout) noexcept -> void
		{
			CHECK_WIN32(WaitForSingleObject(value, timeout));
		}

		static auto unlock(pointer value) noexcept -> void
		{
			VERIFY(ReleaseMutex(value));
		}
	};

	struct semaphore_lock_guard_traits
	{
		typedef HANDLE pointer;

		static auto lock(pointer value, unsigned long timeout) noexcept -> void
		{
			CHECK_WIN32(WaitForSingleObject(value, timeout));
		}

		static auto unlock(pointer value) noexcept -> void
		{
			long count;
			VERIFY(ReleaseSemaphore(value, 1L, &count));
		}
	};

	template <typename Traits>
	class basic_lock_guard
	{
	protected:
		typedef typename Traits::pointer pointer;

	public:
		explicit basic_lock_guard(pointer value, unsigned long timeout = INFINITE, bool owned = false) noexcept
			: m_value{ value }
		{
			if (!owned)
			{
				Traits::lock(m_value, timeout);
			}
		}

		virtual ~basic_lock_guard() noexcept
		{
			Traits::unlock(m_value);
		}

	private:
		pointer m_value;
	};

	typedef basic_lock_guard<mutex_lock_guard_traits> mutex_lock_guard;
	typedef basic_lock_guard<semaphore_lock_guard_traits> semaphore_lock_guard;
}
