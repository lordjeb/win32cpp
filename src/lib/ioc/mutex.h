#pragma once
#include <Windows.h>

namespace win32cpp
{
	inline namespace ioc
	{
		class IMutex
		{
		public:
			virtual auto CloseHandle(_In_ HANDLE hObject)->BOOL = 0;

			virtual auto CreateMutexA(
				_In_opt_ LPSECURITY_ATTRIBUTES lpMutexAttributes,
				_In_ BOOL bInitialOwner,
				_In_opt_ LPCSTR lpName
			)->HANDLE = 0;

			virtual auto CreateMutexW(
				_In_opt_ LPSECURITY_ATTRIBUTES lpMutexAttributes,
				_In_ BOOL bInitialOwner,
				_In_opt_ LPCWSTR lpName
			)->HANDLE = 0;

			virtual DWORD GetLastError() = 0;

			virtual auto OpenMutexA(
				_In_ DWORD dwDesiredAccess,
				_In_ BOOL bInheritHandle,
				_In_ LPCSTR lpName
			)->HANDLE = 0;

			virtual auto OpenMutexW(
				_In_ DWORD dwDesiredAccess,
				_In_ BOOL bInheritHandle,
				_In_ LPCWSTR lpName
			)->HANDLE = 0;

			virtual auto ReleaseMutex(_In_ HANDLE hMutex)->BOOL = 0;
		};

		class MutexImpl : public IMutex
		{
		public:
			auto CloseHandle(_In_ HANDLE hObject)->BOOL override;

			auto CreateMutexA(
				_In_opt_ LPSECURITY_ATTRIBUTES lpMutexAttributes,
				_In_ BOOL bInitialOwner,
				_In_opt_ LPCSTR lpName
			)->HANDLE override;

			auto CreateMutexW(
				_In_opt_ LPSECURITY_ATTRIBUTES lpMutexAttributes,
				_In_ BOOL bInitialOwner,
				_In_opt_ LPCWSTR lpName
			)->HANDLE override;

			DWORD GetLastError() override;

			auto OpenMutexA(
				_In_ DWORD dwDesiredAccess,
				_In_ BOOL bInheritHandle,
				_In_ LPCSTR lpName
			)->HANDLE override;

			auto OpenMutexW(
				_In_ DWORD dwDesiredAccess,
				_In_ BOOL bInheritHandle,
				_In_ LPCWSTR lpName
			)->HANDLE override;

			auto ReleaseMutex(_In_ HANDLE hMutex)->BOOL override;
		};
	}
}
