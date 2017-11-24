#include "pch.h"
#include "string_extensions.h"

using namespace win32cpp;
using testing::StrEq;

TEST(appendPath_test, appends_n_paths)
{
	ASSERT_THAT(appendPath(L"C:", L"root_dir", L"subdir"), StrEq(LR"(C:\root_dir\subdir)"));
}

TEST(appendPath_test, trims_extra_slashes)
{
	EXPECT_THAT(appendPath(L"C:\\", L"root_dir", L"subdir"), StrEq(LR"(C:\root_dir\subdir)"));
	EXPECT_THAT(appendPath(L"C:\\", L"\\root_dir\\", L"\\subdir"), StrEq(LR"(C:\root_dir\subdir)"));
}

TEST(appendPath_test, leaves_beginning_slashes)
{
	ASSERT_THAT(appendPath(L"\\\\server\\share", L"root_dir", L"subdir"), StrEq(LR"(\\server\share\root_dir\subdir)"));
}

TEST(appendPath_test, trims_ending_slashes)
{
	ASSERT_THAT(appendPath(L"C:", L"root_dir\\"), StrEq(LR"(C:\root_dir)"));
}

TEST(trim_test, handles_empty_string)
{
	ASSERT_THAT(trim(L"", isspace), StrEq(L""));
}

TEST(trim_test, trims_from_beginning)
{
	ASSERT_THAT(trim(L"   test", isspace), StrEq(L"test"));
}

TEST(trim_test, trims_from_end)
{
	ASSERT_THAT(trim(L"test   ", isspace), StrEq(L"test"));
}

TEST(trim_test, trims_from_beginning_and_end)
{
	ASSERT_THAT(trim(L"   test   ", isspace), StrEq(L"test"));
}

TEST(trim_test, trimRight_trims_only_right)
{
	ASSERT_THAT(trimRight(L"   test   ", isspace), StrEq(L"   test"));
}

TEST(trim_test, trimLeft_trims_only_left)
{
	ASSERT_THAT(trimLeft(L"   test   ", isspace), StrEq(L"test   "));
}

TEST(string_conversion_test, converts_wstring_to_string)
{
	ASSERT_THAT(wstr2str(L"test"), StrEq("test"));
}

TEST(string_conversion_test, converts_string_to_wstring)
{
	ASSERT_THAT(str2wstr("test"), StrEq(L"test"));
}
