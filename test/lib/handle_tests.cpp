#include "handle.h"
#include <gmock/gmock.h>
#include "module_info.h"

using namespace win32cpp;
using ::testing::Eq;
using ::testing::Gt;
using ::testing::Ne;

struct basic_unique_handle_test : public ::testing::Test
{
    std::wstring filename = getTempFilename(getTempPath(), L"buh", 1);
};

static_assert(!std::is_copy_constructible<unique_file_handle>::value,
              "unique_file_handle must not be copy-constructable");

static_assert(!std::is_copy_assignable<unique_file_handle>::value, "unique_file_handle must not be copy-assignable");

static_assert(std::is_move_constructible<unique_file_handle>::value, "unique_file_handle must be move-constructable");

static_assert(std::is_move_assignable<unique_file_handle>::value, "unique_file_handle must be move-assignable");

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
    ASSERT_THAT(::CloseHandle(detached_handle), Eq(TRUE));
}

TEST_F(basic_unique_handle_test, registry_handle)
{
    auto h = unique_registry_handle{};
    auto lr = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE", 0, GENERIC_READ, h.get_address_of());
    ASSERT_THAT(lr, Eq(ERROR_SUCCESS));
    ASSERT_THAT((bool)h, Eq(true));
    ASSERT_THAT(h.get(), Ne(nullptr));
}

TEST_F(basic_unique_handle_test, find_handle)
{
    WIN32_FIND_DATA fd{ 0 };
    auto h = unique_find_handle{ ::FindFirstFile(LR"(C:\*)", &fd) };
    ASSERT_THAT((bool)h, Eq(true));
    ASSERT_THAT(wcslen(fd.cFileName), Gt(0UL));
}
