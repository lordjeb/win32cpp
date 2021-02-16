#include "pch.h"
#include "string_extensions.h"

using namespace win32cpp;
using testing::StrEq;

TEST(string_extensions_test, appendPath_appends_empty_string_without_extra_slash)
{
    ASSERT_THAT(appendPath(L"C:\\root_dir", L""), StrEq(LR"(C:\root_dir)"));
}

TEST(string_extensions_test, appendPath_appends_n_paths)
{
    ASSERT_THAT(appendPath(L"C:", L"root_dir", L"subdir"), StrEq(LR"(C:\root_dir\subdir)"));
}

TEST(string_extensions_test, appendPath_trims_extra_slashes_1)
{
    ASSERT_THAT(appendPath(L"C:\\", L"root_dir", L"subdir"), StrEq(LR"(C:\root_dir\subdir)"));
}

TEST(string_extensions_test, appendPath_trims_extra_slashes_2)
{
    ASSERT_THAT(appendPath(L"C:\\", L"\\root_dir\\", L"\\subdir"), StrEq(LR"(C:\root_dir\subdir)"));
}

TEST(string_extensions_test, appendPath_leaves_beginning_slashes)
{
    ASSERT_THAT(appendPath(L"\\\\server\\share", L"root_dir", L"subdir"), StrEq(LR"(\\server\share\root_dir\subdir)"));
}

TEST(string_extensions_test, appendPath_trims_ending_slashes)
{
    ASSERT_THAT(appendPath(L"C:", L"root_dir\\"), StrEq(LR"(C:\root_dir)"));
}

TEST(string_extensions_test, trim_handles_empty_string)
{
    ASSERT_THAT(trim(L"", isspace), StrEq(L""));
}

TEST(string_extensions_test, trim_trims_from_beginning)
{
    ASSERT_THAT(trim(L"   test", isspace), StrEq(L"test"));
}

TEST(string_extensions_test, trim_trims_from_end)
{
    ASSERT_THAT(trim(L"test   ", isspace), StrEq(L"test"));
}

TEST(string_extensions_test, trim_trims_from_beginning_and_end)
{
    ASSERT_THAT(trim(L"   test   ", isspace), StrEq(L"test"));
}

TEST(string_extensions_test, trimRight_trims_only_right)
{
    ASSERT_THAT(trimRight(L"   test   ", isspace), StrEq(L"   test"));
}

TEST(string_extensions_test, trimLeft_trims_only_left)
{
    ASSERT_THAT(trimLeft(L"   test   ", isspace), StrEq(L"test   "));
}

TEST(string_extensions_test, converts_wstring_to_string)
{
    ASSERT_THAT(wstr2str(L"test"), StrEq("test"));
}

TEST(string_extensions_test, converts_string_to_wstring)
{
    ASSERT_THAT(str2wstr("test"), StrEq(L"test"));
}
