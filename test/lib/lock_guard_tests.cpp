#include "pch.h"
#include "lock_guard.h"
#include "handle.h"
#include "windows_constants.h"
#include <Windows.h>

using namespace win32cpp;
using ::testing::Eq;

TEST(lock_guard_test, mutex_can_be_locked)
{
    auto mutex = unique_handle{ ::CreateMutex(NoSecurityAttributes, NotInitiallyOwned, L"MutexName") };
    ASSERT_THAT((bool)mutex, Eq(true));
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
    ASSERT_THAT((bool)mutex_ref, Eq(true));
    ASSERT_THAT(createMutexResult, Eq(DWORD{ ERROR_ALREADY_EXISTS }));
    {
        auto mutex_guard = mutex_lock_guard{ mutex_ref.get(), INFINITE, ERROR_ALREADY_EXISTS != createMutexResult };

        // ... do something with the locked resource
    }
}

TEST(lock_guard_test, ab3)
{
    auto semaphore = unique_handle{ ::CreateSemaphore(NoSecurityAttributes, 1, 1, Unnamed) };
    ASSERT_THAT((bool)semaphore, Eq(true));
    {
        auto semaphore_guard = semaphore_lock_guard{ semaphore.get() };

        // ... do something with the locked resource
    }
}
