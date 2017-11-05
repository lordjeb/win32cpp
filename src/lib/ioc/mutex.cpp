#include "mutex.h"

namespace win32cpp
{
	inline namespace ioc
	{
		auto MutexImpl::CloseHandle(_In_ HANDLE hObject)->BOOL
		{
			return ::CloseHandle(hObject);
		}

		auto MutexImpl::CreateMutexA(
			_In_opt_ LPSECURITY_ATTRIBUTES lpMutexAttributes,
			_In_ BOOL bInitialOwner,
			_In_opt_ LPCSTR lpName
		)->HANDLE
		{
			return ::CreateMutexA(lpMutexAttributes, bInitialOwner, lpName);
		}

		auto MutexImpl::CreateMutexW(
			_In_opt_ LPSECURITY_ATTRIBUTES lpMutexAttributes,
			_In_ BOOL bInitialOwner,
			_In_opt_ LPCWSTR lpName
		)->HANDLE
		{
			return ::CreateMutexW(lpMutexAttributes, bInitialOwner, lpName);
		}

		DWORD MutexImpl::GetLastError()
		{
			return GetLastError();
		}

		auto MutexImpl::OpenMutexA(
			_In_ DWORD dwDesiredAccess,
			_In_ BOOL bInheritHandle,
			_In_ LPCSTR lpName
		)->HANDLE
		{
			return ::OpenMutexA(dwDesiredAccess, bInheritHandle, lpName);
		}

		auto MutexImpl::OpenMutexW(
			_In_ DWORD dwDesiredAccess,
			_In_ BOOL bInheritHandle,
			_In_ LPCWSTR lpName
		)->HANDLE
		{
			return ::OpenMutexW(dwDesiredAccess, bInheritHandle, lpName);
		}

		auto MutexImpl::ReleaseMutex(_In_ HANDLE hMutex)->BOOL
		{
			return ReleaseMutex(hMutex);
		}
	}
}
