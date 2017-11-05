#include "semaphore.h"

namespace win32cpp
{
	inline namespace ioc
	{
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
