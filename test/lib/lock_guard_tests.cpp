#include "pch.h"
#include "lock_guard.h"
#include "handle.h"
#include "windows_constants.h"
#include <Windows.h>

TEST(lock_guard_test, mutex_can_be_locked)
{
    auto mutex = unique_handle{ ::CreateMutex(NoSecurityAttributes, NotInitiallyOwned, L"MutexName") };
    ASSERT_TRUE(mutex);
    {
        auto mutex_guard = mutex_lock_guard{ mutex.get() };

        // ... do something with the locked resource
    }
}

TEST(lock_guard_test, mutex_can_be_created_locked)
{
    // If the mutex is created as initially owned then mutex_lock_guard can be initialized to skip the locking
    auto mutex = unique_handle{ ::CreateMutex(NoSecurityAttributes, NotInitiallyOwned, L"MutexName") };
    auto mutex_ref = unique_handle{ ::CreateMutex(NoSecurityAttributes, InitiallyOwned, L"MutexName") };
    auto createMutexResult = GetLastError();
    ASSERT_TRUE(mutex_ref);
    ASSERT_THAT(createMutexResult, Eq(DWORD{ ERROR_ALREADY_EXISTS }));
    {
        auto mutex_guard = mutex_lock_guard{ mutex_ref.get(), INFINITE, ERROR_ALREADY_EXISTS != createMutexResult };

        // ... do something with the locked resource
    }
}

TEST(lock_guard_test, ab3)
{
    auto semaphore = unique_handle{ ::CreateSemaphore(NoSecurityAttributes, 1, 1, Unnamed) };
    ASSERT_TRUE(semaphore);
    {
        auto semaphore_guard = semaphore_lock_guard{ semaphore.get() };

        // ... do something with the locked resource
    }
}
