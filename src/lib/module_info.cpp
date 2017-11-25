#include "pch.h"
#include "debug.h"
#include "error.h"
#include "module_info.h"

using namespace std;

namespace win32cpp
{
	auto getModuleFilename() -> wstring
	{
		vector<wchar_t> moduleFilenameBuffer(MAX_PATH);
		CHECK_COUNT(GetModuleFileNameW(nullptr, &moduleFilenameBuffer[0], MAX_PATH));
		return &moduleFilenameBuffer[0];
	}

	auto getModulePath() -> wstring
	{
		auto moduleFilename = getModuleFilename();
		return moduleFilename.substr(0, moduleFilename.rfind(L'\\'));
	}

	auto getTempFilename(const wstring& path, const wstring& prefix, unsigned int unique /*= 0*/) -> wstring
	{
		vector<wchar_t> tempFilenameBuffer(MAX_PATH);
		CHECK_COUNT(GetTempFileName(getTempPath().c_str(), prefix.c_str(), unique, &tempFilenameBuffer[0]));
		return &tempFilenameBuffer[0];
	}

	auto getTempPath() -> wstring
	{
		vector<wchar_t> tempPathBuffer(MAX_PATH);
		CHECK_COUNT(GetTempPathW(MAX_PATH, &tempPathBuffer[0]));
		return &tempPathBuffer[0];
	}

	auto getWindowsPath() -> wstring
	{
		vector<wchar_t> tempPathBuffer(MAX_PATH);
		CHECK_COUNT(GetWindowsDirectoryW(&tempPathBuffer[0], MAX_PATH));
		return &tempPathBuffer[0];
	}

	auto getSystemPath() -> wstring
	{
		vector<wchar_t> tempPathBuffer(MAX_PATH);
		CHECK_COUNT(GetSystemDirectoryW(&tempPathBuffer[0], MAX_PATH));
		return &tempPathBuffer[0];
	}

	auto getThreadToken(DWORD DesiredAccess /*= TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY*/) -> unique_token_handle
	{
		HANDLE threadToken;

		if (OpenThreadToken(GetCurrentThread(), DesiredAccess, TRUE, &threadToken))
		{
			return unique_token_handle{ threadToken, false };
		}

		// If the error is something other than ERROR_NO_TOKEN, then it's a real error and we should abort
		auto error = GetLastError();
		if (ERROR_NO_TOKEN != error)
		{
			CHECK_WIN32(error);
		}
		
		CHECK_BOOL(ImpersonateSelf(SecurityImpersonation));
		try
		{
			CHECK_BOOL(OpenThreadToken(GetCurrentThread(), DesiredAccess, TRUE, &threadToken));
			return unique_token_handle{ threadToken, true };
		}
		catch (...)
		{
			RevertToSelf();
			throw;
		}
	}
}
