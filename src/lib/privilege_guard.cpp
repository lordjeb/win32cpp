#include "stdafx.h"
#include "handle.hpp"
#include "privilege_guard.hpp"
#include "module_info.hpp"

using namespace std;

namespace win32cpp
{
	privilege_guard::privilege_guard(const shared_ptr<Imodule_info>& moduleInfo, const wstring& privilege)
		: m_moduleInfo{ moduleInfo }, m_handle{ moduleInfo->getCurrentToken() }, m_privilege{ privilege }
	{
		// TODO: Make this work with only the current thread
		m_moduleInfo->enablePrivilege(m_handle.get(), m_privilege, true);
	}

	privilege_guard::~privilege_guard()
	{
		m_moduleInfo->enablePrivilege(m_handle.get(), m_privilege, false);
	}
}
