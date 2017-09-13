#pragma once
#include "stdafx.h"
#include "module_info.hpp"

namespace win32cpp
{
	class privilege_guard
	{
	public:
		explicit privilege_guard(const std::shared_ptr<Imodule_info>& moduleInfo, const std::wstring& privilege);
		virtual ~privilege_guard();

		privilege_guard(const privilege_guard&) = delete;
		privilege_guard& operator=(const privilege_guard&) = delete;

	private:
		const std::shared_ptr<Imodule_info> m_moduleInfo;
		const unique_handle m_handle;
		const std::wstring m_privilege;
	};
}
