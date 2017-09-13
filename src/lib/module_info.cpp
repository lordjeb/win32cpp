#include "stdafx.h"
#include "module_info.hpp"
#include "windows_constants.hpp"

using namespace std;

namespace win32cpp
{
	module_info::module_info()
	{
	}

	module_info::~module_info()
	{
	}

	void module_info::enablePrivilege(HANDLE tokenHandle, const wstring& privilege, bool enable) const
	{
		TOKEN_PRIVILEGES tokenPrivileges;
		LUID luid;

		CHECK_BOOL(getGlobalWindowsApi()->LookupPrivilegeValueW(nullptr, privilege.c_str(), &luid));

		tokenPrivileges.PrivilegeCount = 1;
		tokenPrivileges.Privileges[0].Luid = luid;
		tokenPrivileges.Privileges[0].Attributes = enable ? SE_PRIVILEGE_ENABLED : 0;

		CHECK_BOOL(getGlobalWindowsApi()->AdjustTokenPrivileges(tokenHandle, FALSE, &tokenPrivileges, sizeof(TOKEN_PRIVILEGES), static_cast<PTOKEN_PRIVILEGES>(nullptr), static_cast<PDWORD>(nullptr)));
		CHECK_WIN32(getGlobalWindowsApi()->GetLastError());
		// TODO: Make this return whether or not the privilege was enabled (so the caller can know that they need to disable it)
	}

	unique_handle module_info::getCurrentToken() const
	{
		unique_handle h;
		if (!getGlobalWindowsApi()->OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES, OpenAsProcess, h.get_address_of()))
		{
			CHECK_BOOL(getGlobalWindowsApi()->OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, h.get_address_of()));
		}
		return h;
	}

	wstring module_info::getModuleFilename() const
	{
		vector<wchar_t> moduleFilenameBuffer(MAX_PATH);
		CHECK_COUNT(getGlobalWindowsApi()->GetModuleFileName(nullptr, &moduleFilenameBuffer[0], MAX_PATH));
		return &moduleFilenameBuffer[0];
	}

	wstring module_info::getModulePath() const
	{
		auto moduleFilename = getModuleFilename();
		return moduleFilename.substr(0, moduleFilename.rfind(L'\\'));
	}
}
