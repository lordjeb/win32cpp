#include "stdafx.h"
#include "windows_api.hpp"

using namespace std;
using namespace vxshared;

namespace vxshared
{
	shared_ptr<Iwindows_api> g_windows_api{ new windows_api() };

	Iwindows_api::~Iwindows_api()
	{
	}

	BOOL windows_api::AdjustTokenPrivileges(_In_ HANDLE TokenHandle, _In_ BOOL DisableAllPrivileges, _In_opt_ PTOKEN_PRIVILEGES NewState, _In_ DWORD BufferLength, _Out_opt_ PTOKEN_PRIVILEGES PreviousState, _Out_opt_ PDWORD ReturnLength)
	{
		return ::AdjustTokenPrivileges(TokenHandle, DisableAllPrivileges, NewState, BufferLength, PreviousState, ReturnLength);
	}

	BOOL windows_api::CloseHandle(_In_ HANDLE hObject)
	{
		return ::CloseHandle(hObject);
	}

	BOOL windows_api::CloseServiceHandle(_In_ SC_HANDLE hSCObject)
	{
		return ::CloseServiceHandle(hSCObject);
	}

	DWORD windows_api::GetLastError()
	{
		return ::GetLastError();
	}

	DWORD windows_api::GetModuleFileNameW(_In_opt_ HMODULE hModule, _Out_ LPWSTR lpFilename, _In_ DWORD nSize)
	{
		return ::GetModuleFileNameW(hModule, lpFilename, nSize);
	}

	BOOL windows_api::LookupPrivilegeValueW(_In_opt_ LPCWSTR lpSystemName, _In_ LPCWSTR lpName, _Out_ PLUID lpLuid)
	{
		return ::LookupPrivilegeValueW(lpSystemName, lpName, lpLuid);
	}

	BOOL windows_api::OpenProcessToken(_In_ HANDLE ProcessHandle, _In_ DWORD DesiredAccess, _Out_ PHANDLE TokenHandle)
	{
		return ::OpenProcessToken(ProcessHandle, DesiredAccess, TokenHandle);
	}

	BOOL windows_api::OpenThreadToken(_In_ HANDLE ThreadHandle, _In_ DWORD DesiredAccess, _In_ BOOL OpenAsSelf, _Out_ PHANDLE TokenHandle)
	{
		return ::OpenThreadToken(ThreadHandle, DesiredAccess, OpenAsSelf, TokenHandle);
	}

	void windows_api::OutputDebugStringW(_In_ LPCWSTR lpOutputString)
	{
		return ::OutputDebugStringW(lpOutputString);
	}

	LSTATUS windows_api::RegCloseKey(_In_ HKEY hKey)
	{
		return ::RegCloseKey(hKey);
	}

	BOOL windows_api::WinHttpCloseHandle(_In_ HINTERNET hInternet)
	{
		return ::WinHttpCloseHandle(hInternet);
	}

	shared_ptr<Iwindows_api> vxshared::getGlobalWindowsApi()
	{
		return g_windows_api;
	}

	void vxshared::setGlobalWindowsApi(const shared_ptr<Iwindows_api> p)
	{
		if (p)
		{
			g_windows_api = p;
		}
		else
		{
			g_windows_api = make_shared<windows_api>();
		}
	}
}
