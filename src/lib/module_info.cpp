#include "error.hpp"
#include "module_info.hpp"
//#include "windows_constants.hpp"

using namespace std;

namespace win32cpp
{
	// void module_info::enablePrivilege(HANDLE tokenHandle, const wstring& privilege, bool enable) const
	// {
	// 	TOKEN_PRIVILEGES tokenPrivileges;
	// 	LUID luid;

	// 	CHECK_BOOL(LookupPrivilegeValueW(nullptr, privilege.c_str(), &luid));

	// 	tokenPrivileges.PrivilegeCount = 1;
	// 	tokenPrivileges.Privileges[0].Luid = luid;
	// 	tokenPrivileges.Privileges[0].Attributes = enable ? SE_PRIVILEGE_ENABLED : 0;

	// 	CHECK_BOOL(AdjustTokenPrivileges(tokenHandle, FALSE, &tokenPrivileges, sizeof(TOKEN_PRIVILEGES), static_cast<PTOKEN_PRIVILEGES>(nullptr), static_cast<PDWORD>(nullptr)));
	// 	CHECK_WIN32(GetLastError());
	// 	// TODO: Make this return whether or not the privilege was enabled (so the caller can know that they need to disable it)
	// }

	// unique_handle module_info::getCurrentToken()
	// {
	// 	unique_handle h;
	// 	if (!OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES, OpenAsProcess, h.get_address_of()))
	// 	{
	// 		CHECK_BOOL(OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, h.get_address_of()));
	// 	}
	// 	return h;
	// }

	wstring module_info::getModuleFilename()
	{
		vector<wchar_t> moduleFilenameBuffer(MAX_PATH);
		CHECK_COUNT(GetModuleFileNameW(nullptr, &moduleFilenameBuffer[0], MAX_PATH));
		return &moduleFilenameBuffer[0];
	}

	wstring module_info::getModulePath()
	{
		auto moduleFilename = getModuleFilename();
		return moduleFilename.substr(0, moduleFilename.rfind(L'\\'));
	}

	wstring module_info::getTempPath()
	{
		vector<wchar_t> tempPathBuffer(MAX_PATH);
		CHECK_COUNT(GetTempPathW(MAX_PATH, &tempPathBuffer[0]));
		return &tempPathBuffer[0];
	}
}
