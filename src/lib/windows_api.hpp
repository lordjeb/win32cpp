#pragma once
#include "stdafx.h"

namespace vxshared
{
	class Iwindows_api
	{
	public:
		virtual ~Iwindows_api();

		virtual BOOL AdjustTokenPrivileges(_In_ HANDLE TokenHandle, _In_ BOOL DisableAllPrivileges, _In_opt_ PTOKEN_PRIVILEGES NewState, _In_ DWORD BufferLength, _Out_opt_ PTOKEN_PRIVILEGES PreviousState, _Out_opt_ PDWORD ReturnLength) = 0;
		virtual BOOL CloseHandle(_In_ HANDLE hObject) = 0;
		virtual BOOL CloseServiceHandle(_In_ SC_HANDLE hSCObject) = 0;
		virtual DWORD GetLastError() = 0;
		virtual DWORD GetModuleFileNameW(_In_opt_ HMODULE hModule, _Out_ LPWSTR lpFilename, _In_ DWORD nSize) = 0;
		virtual BOOL LookupPrivilegeValueW(_In_opt_ LPCTSTR lpSystemName, _In_ LPCTSTR lpName, _Out_ PLUID lpLuid) = 0;
		virtual BOOL OpenProcessToken(_In_ HANDLE ProcessHandle, _In_ DWORD DesiredAccess, _Out_ PHANDLE TokenHandle) = 0;
		virtual BOOL OpenThreadToken(_In_ HANDLE ThreadHandle, _In_ DWORD DesiredAccess, _In_ BOOL OpenAsSelf, _Out_ PHANDLE TokenHandle) = 0;
		virtual void OutputDebugStringW(_In_ LPCWSTR lpOutputString) = 0;
		virtual LSTATUS RegCloseKey(_In_ HKEY hKey) = 0;
		virtual BOOL WinHttpCloseHandle(_In_ HINTERNET hInternet) = 0;
	};

	class windows_api : public Iwindows_api
	{
	public:
		BOOL AdjustTokenPrivileges(_In_ HANDLE TokenHandle, _In_ BOOL DisableAllPrivileges, _In_opt_ PTOKEN_PRIVILEGES NewState, _In_ DWORD BufferLength, _Out_opt_ PTOKEN_PRIVILEGES PreviousState, _Out_opt_ PDWORD ReturnLength) override;
		BOOL CloseHandle(_In_ HANDLE hObject) override;
		BOOL CloseServiceHandle(_In_ SC_HANDLE hSCObject) override;
		DWORD GetLastError() override;
		DWORD GetModuleFileNameW(_In_opt_ HMODULE hModule, _Out_ LPWSTR lpFilename, _In_ DWORD nSize) override;
		BOOL LookupPrivilegeValueW(_In_opt_ LPCWSTR lpSystemName, _In_ LPCWSTR lpName, _Out_ PLUID lpLuid) override;
		BOOL OpenProcessToken(_In_ HANDLE ProcessHandle, _In_ DWORD DesiredAccess, _Out_ PHANDLE TokenHandle) override;
		BOOL OpenThreadToken(_In_ HANDLE ThreadHandle, _In_ DWORD DesiredAccess, _In_ BOOL OpenAsSelf, _Out_ PHANDLE TokenHandle) override;
		void OutputDebugStringW(_In_ LPCWSTR lpOutputString) override;
		LSTATUS RegCloseKey(_In_ HKEY hKey) override;
		BOOL WinHttpCloseHandle(_In_ HINTERNET hInternet) override;
	};

	std::shared_ptr<Iwindows_api> getGlobalWindowsApi();
	void setGlobalWindowsApi(const std::shared_ptr<Iwindows_api> p);
}
