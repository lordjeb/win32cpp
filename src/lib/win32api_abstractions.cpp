#include "pch.h"
#include "win32api_abstractions.h"

namespace win32cpp
{
	inline namespace ioc
	{
		auto ErrorImpl::GetLastError() ->DWORD
		{
			return ::GetLastError();
		}

		auto FileImpl::CloseHandle(_In_ HANDLE hObject) ->BOOL
		{
			return ::CloseHandle(hObject);
		}

		auto FileImpl::CreateFileA(_In_ LPCSTR lpFileName, _In_ DWORD dwDesiredAccess, _In_ DWORD dwShareMode, _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes, _In_ DWORD dwCreationDisposition, _In_ DWORD dwFlagsAndAttributes, _In_opt_ HANDLE hTemplateFile) ->HANDLE
		{
			return ::CreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
		}

		auto FileImpl::CreateFileW(_In_ LPCWSTR lpFileName, _In_ DWORD dwDesiredAccess, _In_ DWORD dwShareMode, _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes, _In_ DWORD dwCreationDisposition, _In_ DWORD dwFlagsAndAttributes, _In_opt_ HANDLE hTemplateFile) ->HANDLE
		{
			return ::CreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
		}

		auto FileImpl::CreateFileMappingA(_In_ HANDLE hFile, _In_opt_ LPSECURITY_ATTRIBUTES lpFileMappingAttributes, _In_ DWORD flProtect, _In_ DWORD dwMaximumSizeHigh, _In_ DWORD dwMaximumSizeLow, _In_opt_ LPCSTR lpName) ->HANDLE
		{
			return ::CreateFileMappingA(hFile, lpFileMappingAttributes, flProtect, dwMaximumSizeHigh, dwMaximumSizeLow, lpName);
		}

		auto FileImpl::CreateFileMappingW(_In_ HANDLE hFile, _In_opt_ LPSECURITY_ATTRIBUTES lpFileMappingAttributes, _In_ DWORD flProtect, _In_ DWORD dwMaximumSizeHigh, _In_ DWORD dwMaximumSizeLow, _In_opt_ LPCWSTR lpName) ->HANDLE
		{
			return ::CreateFileMappingW(hFile, lpFileMappingAttributes, flProtect, dwMaximumSizeHigh, dwMaximumSizeLow, lpName);
		}

		auto FileImpl::MapViewOfFile(_In_ HANDLE hFileMappingObject, _In_ DWORD dwDesiredAccess, _In_ DWORD dwFileOffsetHigh, _In_ DWORD dwFileOffsetLow, _In_ SIZE_T dwNumberOfBytesToMap) ->LPVOID
		{
			return ::MapViewOfFile(hFileMappingObject, dwDesiredAccess, dwFileOffsetHigh, dwFileOffsetLow, dwNumberOfBytesToMap);
		}

		auto FileImpl::UnmapViewOfFile(_In_ LPCVOID lpBaseAddress) ->BOOL
		{
			return ::UnmapViewOfFile(lpBaseAddress);
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
