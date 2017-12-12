#include "pch.h"
#include "handle.h"
#include "module_info.h"

struct basic_unique_handle_test : public ::testing::Test
{
    std::wstring filename = getTempFilename(getTempPath(), L"buh", 1);
};

TEST_F(basic_unique_handle_test, is_noncopyable_and_moveable)
{
    ASSERT_FALSE(std::is_copy_constructible<unique_file_handle>::value);
    ASSERT_FALSE(std::is_copy_assignable<unique_file_handle>::value);
    ASSERT_TRUE(std::is_move_constructible<unique_file_handle>::value);
    ASSERT_TRUE(std::is_move_assignable<unique_file_handle>::value);
}

TEST_F(basic_unique_handle_test, closes_handle)
{
    {
        auto h = unique_file_handle{ ::CreateFile(filename.c_str(), GENERIC_ALL, 0, nullptr, CREATE_NEW,
                                                  FILE_FLAG_DELETE_ON_CLOSE, nullptr) };
    }
    ASSERT_THAT(::GetFileAttributes(filename.c_str()), Eq(0xFFFFFFFF));
}

TEST_F(basic_unique_handle_test, doesnt_close_invalid_handle)
{
    auto h = unique_file_handle{ INVALID_HANDLE_VALUE };
    // NOTE: No ASSERT, because the call to CloseHandle will assert if called with an invalid handle
}

TEST_F(basic_unique_handle_test, closes_handle_when_reset)
{
    auto h = unique_file_handle{ ::CreateFile(filename.c_str(), GENERIC_ALL, 0, nullptr, CREATE_NEW,
                                              FILE_FLAG_DELETE_ON_CLOSE, nullptr) };
    h.reset(INVALID_HANDLE_VALUE);
    ASSERT_THAT(::GetFileAttributes(filename.c_str()), Eq(0xFFFFFFFF));
    ASSERT_THAT(h.get(), Eq(INVALID_HANDLE_VALUE));
}

TEST_F(basic_unique_handle_test, doesnt_close_released_handle)
{
    auto detached_handle = HANDLE{ INVALID_HANDLE_VALUE };
    {
        unique_file_handle h{ ::CreateFile(filename.c_str(), GENERIC_ALL, 0, nullptr, CREATE_NEW,
                                           FILE_FLAG_DELETE_ON_CLOSE, nullptr) };
        detached_handle = h.release();
    }
    ASSERT_THAT(::GetFileAttributes(filename.c_str()), Ne(0xFFFFFFFF));
    ASSERT_TRUE(::CloseHandle(detached_handle));
}

TEST_F(basic_unique_handle_test, registry_handle)
{
    auto h = unique_registry_handle{};
    auto lr = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE", 0, GENERIC_READ, h.get_address_of());
    ASSERT_THAT(lr, Eq(ERROR_SUCCESS));
    ASSERT_TRUE(h);
    ASSERT_THAT(h.get(), Ne(nullptr));
}

TEST_F(basic_unique_handle_test, find_handle)
{
    auto fd = WIN32_FIND_DATA{ 0 };
    auto h = unique_find_handle{ ::FindFirstFile(LR"(C:\*)", &fd) };
    ASSERT_TRUE(h);
    ASSERT_THAT(wcslen(fd.cFileName), Gt(0UL));
}
