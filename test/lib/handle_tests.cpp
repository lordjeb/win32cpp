#include "pch.h"
#include "handle.h"
#include "module_info.h"
#include "string_extensions.h"

using namespace win32cpp;
using testing::Eq;
using testing::Ne;

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
		unique_file_handle h{ ::CreateFile(filename.c_str(), GENERIC_ALL, 0, nullptr, CREATE_NEW, FILE_FLAG_DELETE_ON_CLOSE, nullptr) };
	}
	ASSERT_THAT(::GetFileAttributes(filename.c_str()), Eq(0xFFFFFFFF));
}

TEST_F(basic_unique_handle_test, doesnt_close_invalid_handle)
{
	unique_file_handle h{ INVALID_HANDLE_VALUE };
	// NOTE: No ASSERT, because the call to CloseHandle will assert if called with an invalid handle
}

TEST_F(basic_unique_handle_test, closes_handle_when_reset)
{
	unique_file_handle h{ ::CreateFile(filename.c_str(), GENERIC_ALL, 0, nullptr, CREATE_NEW, FILE_FLAG_DELETE_ON_CLOSE, nullptr) };
	h.reset(INVALID_HANDLE_VALUE);
	ASSERT_THAT(::GetFileAttributes(filename.c_str()), Eq(0xFFFFFFFF));
	ASSERT_THAT(h.get(), Eq(INVALID_HANDLE_VALUE));
}

TEST_F(basic_unique_handle_test, doesnt_close_released_handle)
{
	HANDLE detached_handle;
	{
		unique_file_handle h{ ::CreateFile(filename.c_str(), GENERIC_ALL, 0, nullptr, CREATE_NEW, FILE_FLAG_DELETE_ON_CLOSE, nullptr) };
		detached_handle = h.release();
	}
	ASSERT_THAT(::GetFileAttributes(filename.c_str()), Ne(0xFFFFFFFF));
	ASSERT_TRUE(::CloseHandle(detached_handle));
}
