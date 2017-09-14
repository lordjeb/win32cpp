#include "privilege_guard.hpp"
#include "error.hpp"
#include "handle.hpp"

using namespace std;

namespace win32cpp
{
	privilege_guard::privilege_guard(HANDLE tokenHandle, const wstring& privilege)
		: m_tokenHandle{ tokenHandle }, m_privilege{ privilege }
	{
		m_enabled = enablePrivilege(m_tokenHandle, m_privilege, true);
	}

	privilege_guard::privilege_guard(privilege_guard&& src)
		: m_tokenHandle{ src.m_tokenHandle }, m_privilege{ src.m_privilege }, m_enabled{ src.m_enabled }
	{
		src.m_tokenHandle = nullptr;
		src.m_enabled = false;
	}

	privilege_guard::~privilege_guard()
	{
		if (m_enabled)
		{
			enablePrivilege(m_tokenHandle, m_privilege, false);
		}
	}

	auto privilege_guard::operator=(privilege_guard&& src) -> privilege_guard&
	{
		if (this != &src)
		{
			if (m_enabled)
			{
				enablePrivilege(m_tokenHandle, m_privilege, false);
			}
		
			m_tokenHandle = src.m_tokenHandle;
			m_privilege = src.m_privilege;
			m_enabled = src.m_enabled;
	
			src.m_tokenHandle = nullptr;
			src.m_enabled = false;
		}
	
		return *this;
	}

	// Returns true if the privilege was enabled
	auto privilege_guard::enablePrivilege(HANDLE tokenHandle, const std::wstring& privilege, bool enable) -> bool
	{
		LUID luid;
		CHECK_BOOL(LookupPrivilegeValueW(nullptr, privilege.c_str(), &luid));

		TOKEN_PRIVILEGES tokenPrivileges;
		tokenPrivileges.PrivilegeCount = 1;
		tokenPrivileges.Privileges[0].Luid = luid;
		tokenPrivileges.Privileges[0].Attributes = enable ? SE_PRIVILEGE_ENABLED : 0;

		TOKEN_PRIVILEGES originalState;
		DWORD cbOriginalState;
		CHECK_BOOL(AdjustTokenPrivileges(tokenHandle, FALSE, &tokenPrivileges, sizeof(TOKEN_PRIVILEGES),
					&originalState, &cbOriginalState));
		CHECK_WIN32(GetLastError());

		return originalState.PrivilegeCount > 0;
	}
}
