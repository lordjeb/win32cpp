#pragma once
#include <Windows.h>

namespace win32cpp
{
	inline namespace ioc
	{
		class IError
		{
		public:
			virtual auto GetLastError()->DWORD = 0;
		};

		class IFileMapping
		{
		public:
			virtual auto CreateFileMappingA(_In_ HANDLE hFile, _In_opt_ LPSECURITY_ATTRIBUTES lpFileMappingAttributes, _In_ DWORD flProtect, _In_ DWORD dwMaximumSizeHigh, _In_ DWORD dwMaximumSizeLow, _In_opt_ LPCSTR lpName)->HANDLE = 0;
			virtual auto CreateFileMappingW(_In_ HANDLE hFile, _In_opt_ LPSECURITY_ATTRIBUTES lpFileMappingAttributes, _In_ DWORD flProtect, _In_ DWORD dwMaximumSizeHigh, _In_ DWORD dwMaximumSizeLow, _In_opt_ LPCWSTR lpName)->HANDLE = 0;
			virtual auto MapViewOfFile(_In_ HANDLE hFileMappingObject, _In_ DWORD dwDesiredAccess, _In_ DWORD dwFileOffsetHigh, _In_ DWORD dwFileOffsetLow, _In_ SIZE_T dwNumberOfBytesToMap)->LPVOID = 0;
			virtual auto UnmapViewOfFile(_In_ LPCVOID lpBaseAddress)->BOOL = 0;
		};

		class IHandle
		{
		public:
			virtual auto CloseHandle(_In_ HANDLE hObject)->BOOL = 0;
		};

		class IMutex
		{
		public:
			virtual auto CreateMutexA(_In_opt_ LPSECURITY_ATTRIBUTES lpMutexAttributes, _In_ BOOL bInitialOwner, _In_opt_ LPCSTR lpName)->HANDLE = 0;
			virtual auto CreateMutexW(_In_opt_ LPSECURITY_ATTRIBUTES lpMutexAttributes, _In_ BOOL bInitialOwner, _In_opt_ LPCWSTR lpName)->HANDLE = 0;
			virtual auto OpenMutexA(_In_ DWORD dwDesiredAccess, _In_ BOOL bInheritHandle, _In_ LPCSTR lpName)->HANDLE = 0;
			virtual auto OpenMutexW(_In_ DWORD dwDesiredAccess, _In_ BOOL bInheritHandle, _In_ LPCWSTR lpName)->HANDLE = 0;
			virtual auto ReleaseMutex(_In_ HANDLE hMutex)->BOOL = 0;
		};

		class ISemaphore
		{
		public:
			virtual auto CreateSemaphoreA(_In_opt_ LPSECURITY_ATTRIBUTES lpSemaphoreAttributes, _In_ LONG lInitialCount, _In_ LONG lMaximumCount, _In_opt_ LPCSTR lpName)->HANDLE = 0;
			virtual auto CreateSemaphoreW(_In_opt_ LPSECURITY_ATTRIBUTES lpSemaphoreAttributes, _In_ LONG lInitialCount, _In_ LONG lMaximumCount, _In_opt_ LPCWSTR lpName)->HANDLE = 0;
			virtual auto ReleaseSemaphore(_In_ HANDLE hSemaphore, _In_ LONG lReleaseCount, _Out_opt_ LPLONG lpPreviousCount)->BOOL = 0;
		};

		class ErrorImpl : public IError
		{
		public:
			virtual auto GetLastError()->DWORD override;
		};

		class FileMappingImpl : public IFileMapping
		{
		public:
			virtual auto CreateFileMappingA(_In_ HANDLE hFile, _In_opt_ LPSECURITY_ATTRIBUTES lpFileMappingAttributes, _In_ DWORD flProtect, _In_ DWORD dwMaximumSizeHigh, _In_ DWORD dwMaximumSizeLow, _In_opt_ LPCSTR lpName)->HANDLE override;
			virtual auto CreateFileMappingW(_In_ HANDLE hFile, _In_opt_ LPSECURITY_ATTRIBUTES lpFileMappingAttributes, _In_ DWORD flProtect, _In_ DWORD dwMaximumSizeHigh, _In_ DWORD dwMaximumSizeLow, _In_opt_ LPCWSTR lpName)->HANDLE override;
			virtual auto MapViewOfFile(_In_ HANDLE hFileMappingObject, _In_ DWORD dwDesiredAccess, _In_ DWORD dwFileOffsetHigh, _In_ DWORD dwFileOffsetLow, _In_ SIZE_T dwNumberOfBytesToMap)->LPVOID override;
			virtual auto UnmapViewOfFile(_In_ LPCVOID lpBaseAddress)->BOOL override;
		};

		class HandleImpl : public IHandle
		{
		public:
			virtual auto CloseHandle(_In_ HANDLE hObject)->BOOL override;
		};

		class MutexImpl : public IMutex
		{
		public:
			virtual auto CreateMutexA(_In_opt_ LPSECURITY_ATTRIBUTES lpMutexAttributes, _In_ BOOL bInitialOwner, _In_opt_ LPCSTR lpName)->HANDLE override;
			virtual auto CreateMutexW(_In_opt_ LPSECURITY_ATTRIBUTES lpMutexAttributes, _In_ BOOL bInitialOwner, _In_opt_ LPCWSTR lpName)->HANDLE override;
			virtual auto OpenMutexA(_In_ DWORD dwDesiredAccess, _In_ BOOL bInheritHandle, _In_ LPCSTR lpName)->HANDLE override;
			virtual auto OpenMutexW(_In_ DWORD dwDesiredAccess, _In_ BOOL bInheritHandle, _In_ LPCWSTR lpName)->HANDLE override;
			virtual auto ReleaseMutex(_In_ HANDLE hMutex)->BOOL override;
		};

		class SemaphoreImpl : public ISemaphore
		{
		public:
			virtual auto CreateSemaphoreA(_In_opt_ LPSECURITY_ATTRIBUTES lpSemaphoreAttributes, _In_ LONG lInitialCount, _In_ LONG lMaximumCount, _In_opt_ LPCSTR lpName)->HANDLE override;
			virtual auto CreateSemaphoreW(_In_opt_ LPSECURITY_ATTRIBUTES lpSemaphoreAttributes, _In_ LONG lInitialCount, _In_ LONG lMaximumCount, _In_opt_ LPCWSTR lpName)->HANDLE override;
			virtual auto ReleaseSemaphore(_In_ HANDLE hSemaphore, _In_ LONG lReleaseCount, _Out_opt_ LPLONG lpPreviousCount)->BOOL override;
		};
	}
}
