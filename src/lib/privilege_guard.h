#pragma once
#include "handle.h"
#include <string>
#include <windows.h>

namespace win32cpp
{
	class privilege_guard
	{
	public:
		explicit privilege_guard(HANDLE tokenHandle, const std::wstring& privilege);
		virtual ~privilege_guard();

		// Copy (deleted)
		privilege_guard(const privilege_guard&) = delete;
		privilege_guard& operator=(const privilege_guard&) = delete;

		// Move
		privilege_guard(privilege_guard&& src);
		auto operator=(privilege_guard&& src) -> privilege_guard&;
		
	private:
		HANDLE m_tokenHandle;
		std::wstring m_privilege;
		bool m_enabled;

		auto enablePrivilege(HANDLE tokenHandle, const std::wstring& privilege, bool enable) -> bool;
	};
}
