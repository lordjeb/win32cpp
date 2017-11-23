#include "win32api_abstractions.h"

namespace win32cpp
{
	inline namespace ioc
	{
		auto ErrorImpl::GetLastError() ->DWORD
		{
			return ::GetLastError();
		}

		auto FileMappingImpl::CreateFileMappingA(_In_ HANDLE hFile, _In_opt_ LPSECURITY_ATTRIBUTES lpFileMappingAttributes, _In_ DWORD flProtect, _In_ DWORD dwMaximumSizeHigh, _In_ DWORD dwMaximumSizeLow, _In_opt_ LPCSTR lpName) ->HANDLE
		{
			return ::CreateFileMappingA(hFile, lpFileMappingAttributes, flProtect, dwMaximumSizeHigh, dwMaximumSizeLow, lpName);
		}

		auto FileMappingImpl::CreateFileMappingW(_In_ HANDLE hFile, _In_opt_ LPSECURITY_ATTRIBUTES lpFileMappingAttributes, _In_ DWORD flProtect, _In_ DWORD dwMaximumSizeHigh, _In_ DWORD dwMaximumSizeLow, _In_opt_ LPCWSTR lpName) ->HANDLE
		{
			return ::CreateFileMappingW(hFile, lpFileMappingAttributes, flProtect, dwMaximumSizeHigh, dwMaximumSizeLow, lpName);
		}

		auto FileMappingImpl::MapViewOfFile(_In_ HANDLE hFileMappingObject, _In_ DWORD dwDesiredAccess, _In_ DWORD dwFileOffsetHigh, _In_ DWORD dwFileOffsetLow, _In_ SIZE_T dwNumberOfBytesToMap) ->LPVOID
		{
			return ::MapViewOfFile(hFileMappingObject, dwDesiredAccess, dwFileOffsetHigh, dwFileOffsetLow, dwNumberOfBytesToMap);
		}

		auto FileMappingImpl::UnmapViewOfFile(_In_ LPCVOID lpBaseAddress) ->BOOL
		{
			return ::UnmapViewOfFile(lpBaseAddress);
		}

		auto HandleImpl::CloseHandle(_In_ HANDLE hObject) ->BOOL
		{
			return ::CloseHandle(hObject);
		}

		auto MutexImpl::CreateMutexA(_In_opt_ LPSECURITY_ATTRIBUTES lpMutexAttributes, _In_ BOOL bInitialOwner, _In_opt_ LPCSTR lpName) ->HANDLE
		{
			return ::CreateMutexA(lpMutexAttributes, bInitialOwner, lpName);
		}

		auto MutexImpl::CreateMutexW(_In_opt_ LPSECURITY_ATTRIBUTES lpMutexAttributes, _In_ BOOL bInitialOwner, _In_opt_ LPCWSTR lpName) ->HANDLE
		{
			return ::CreateMutexW(lpMutexAttributes, bInitialOwner, lpName);
		}

		auto MutexImpl::OpenMutexA(_In_ DWORD dwDesiredAccess, _In_ BOOL bInheritHandle, _In_ LPCSTR lpName) ->HANDLE
		{
			return ::OpenMutexA(dwDesiredAccess, bInheritHandle, lpName);
		}

		auto MutexImpl::OpenMutexW(_In_ DWORD dwDesiredAccess, _In_ BOOL bInheritHandle, _In_ LPCWSTR lpName) ->HANDLE
		{
			return ::OpenMutexW(dwDesiredAccess, bInheritHandle, lpName);
		}

		auto MutexImpl::ReleaseMutex(_In_ HANDLE hMutex) ->BOOL
		{
			return ::ReleaseMutex(hMutex);
		}

		auto SemaphoreImpl::CreateSemaphoreA(_In_opt_ LPSECURITY_ATTRIBUTES lpSemaphoreAttributes, _In_ LONG lInitialCount, _In_ LONG lMaximumCount, _In_opt_ LPCSTR lpName) ->HANDLE
		{
			return ::CreateSemaphoreA(lpSemaphoreAttributes, lInitialCount, lMaximumCount, lpName);
		}

		auto SemaphoreImpl::CreateSemaphoreW(_In_opt_ LPSECURITY_ATTRIBUTES lpSemaphoreAttributes, _In_ LONG lInitialCount, _In_ LONG lMaximumCount, _In_opt_ LPCWSTR lpName) ->HANDLE
		{
			return ::CreateSemaphoreW(lpSemaphoreAttributes, lInitialCount, lMaximumCount, lpName);
		}

		auto SemaphoreImpl::ReleaseSemaphore(_In_ HANDLE hSemaphore, _In_ LONG lReleaseCount, _Out_opt_ LPLONG lpPreviousCount) ->BOOL
		{
			return ::ReleaseSemaphore(hSemaphore, lReleaseCount, lpPreviousCount);
		}
	}
}
