#pragma once
#include "debug.hpp"
#include "error.hpp"
#include <windows.h>

namespace win32cpp
{
	struct mutex_lock_guard_traits
	{
		typedef HANDLE pointer;

		static auto lock(pointer value, unsigned long timeout) throw() -> void
		{
			CHECK_WIN32(WaitForSingleObject(value, timeout));
		}

		static auto unlock(pointer value) throw() -> void
		{
			VERIFY(ReleaseMutex(value));
		}
	};

	template <typename Traits>
	class basic_lock_guard
	{
	protected:
		typedef typename Traits::pointer pointer;

	public:
		explicit basic_lock_guard(pointer value, unsigned long timeout = INFINITE, bool owned = false) throw()
			: m_value{ value }
		{
			if (!owned)
			{
				Traits::lock(m_value, timeout);
			}
		}

		virtual ~basic_lock_guard() throw()
		{
			Traits::unlock(m_value);
		}

	private:
		pointer m_value;
	};

	typedef basic_lock_guard<mutex_lock_guard_traits> mutex_lock_guard;
}
