#pragma once
#include <Windows.h>

namespace win32cpp
{
    inline namespace ioc
    {
        class ISemaphore
        {
        public:
			virtual auto CreateSemaphoreA(_In_opt_ LPSECURITY_ATTRIBUTES lpSemaphoreAttributes, _In_ LONG lInitialCount, _In_ LONG lMaximumCount, _In_opt_ LPCSTR lpName)->HANDLE = 0;
			virtual auto CreateSemaphoreW(_In_opt_ LPSECURITY_ATTRIBUTES lpSemaphoreAttributes, _In_ LONG lInitialCount, _In_ LONG lMaximumCount, _In_opt_ LPCWSTR lpName)->HANDLE = 0;
			virtual auto ReleaseSemaphore(_In_ HANDLE hSemaphore, _In_ LONG lReleaseCount, _Out_opt_ LPLONG lpPreviousCount)->BOOL = 0;
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
