#pragma once
#include <Windows.h>

namespace win32cpp
{
	inline namespace ioc
	{
		class IMutex
		{
		public:
			virtual auto CreateMutexA(_In_opt_ LPSECURITY_ATTRIBUTES lpMutexAttributes, _In_ BOOL bInitialOwner, _In_opt_ LPCSTR lpName)->HANDLE = 0;
			virtual auto CreateMutexW(_In_opt_ LPSECURITY_ATTRIBUTES lpMutexAttributes, _In_ BOOL bInitialOwner, _In_opt_ LPCWSTR lpName)->HANDLE = 0;
			virtual auto OpenMutexA(_In_ DWORD dwDesiredAccess, _In_ BOOL bInheritHandle, _In_ LPCSTR lpName)->HANDLE = 0;
			virtual auto OpenMutexW(_In_ DWORD dwDesiredAccess, _In_ BOOL bInheritHandle, _In_ LPCWSTR lpName)->HANDLE = 0;
			virtual auto ReleaseMutex(_In_ HANDLE hMutex)->BOOL = 0;
		};

		class MutexImpl : public IMutex
		{
		public:
			virtual auto CreateMutexA(_In_opt_ LPSECURITY_ATTRIBUTES lpMutexAttributes, _In_ BOOL bInitialOwner, _In_opt_ LPCSTR lpName)->HANDLE override
			{
				return ::CreateMutexA(lpMutexAttributes, bInitialOwner, lpName);
			}

			virtual auto CreateMutexW(_In_opt_ LPSECURITY_ATTRIBUTES lpMutexAttributes, _In_ BOOL bInitialOwner, _In_opt_ LPCWSTR lpName)->HANDLE override
			{
				return ::CreateMutexW(lpMutexAttributes, bInitialOwner, lpName);
			}

			virtual auto OpenMutexA(_In_ DWORD dwDesiredAccess, _In_ BOOL bInheritHandle, _In_ LPCSTR lpName)->HANDLE override
			{
				return ::OpenMutexA(dwDesiredAccess, bInheritHandle, lpName);
			}

			virtual auto OpenMutexW(_In_ DWORD dwDesiredAccess, _In_ BOOL bInheritHandle, _In_ LPCWSTR lpName)->HANDLE override
			{
				return ::OpenMutexW(dwDesiredAccess, bInheritHandle, lpName);
			}

			virtual auto ReleaseMutex(_In_ HANDLE hMutex)->BOOL override
			{
				return ::ReleaseMutex(hMutex);
			}
		};
	}
}
