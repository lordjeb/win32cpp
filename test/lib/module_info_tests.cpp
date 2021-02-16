#include "pch.h"
#include "module_info.h"

using namespace win32cpp;
using ::testing::EndsWith;
using ::testing::Eq;
using ::testing::Gt;

TEST(module_info_test, getModuleFilename)
{
    ASSERT_THAT(getModuleFilename(), EndsWith(L"lib_tests.exe"));
}

TEST(module_info_test, getModulePath)
{
    ASSERT_THAT(getModulePath().size(), Gt(0UL));
}

TEST(module_info_test, getTempPath)
{
    ASSERT_THAT(getTempPath().size(), Gt(0UL));
}

TEST(module_info_test, getTempFilename)
{
    auto temp_file = getTempFilename(getTempPath(), L"pre");
    ASSERT_THAT(temp_file.size(), Gt(0UL));
    ASSERT_THAT(::DeleteFile(temp_file.c_str()), Eq(TRUE));
}

TEST(module_info_test, getWindowsPath)
{
    ASSERT_THAT(getWindowsPath().size(), Gt(0UL));
}

TEST(module_info_test, getSystemPath)
{
    ASSERT_THAT(getSystemPath().size(), Gt(0UL));
}

auto isThreadImpersonating() -> bool
{
    auto th = unique_token_handle{};
    return (TRUE == ::OpenThreadToken(GetCurrentThread(), TOKEN_QUERY, TRUE, th.get_address_of()));
}

TEST(module_info_test, getThreadToken)
{
    ASSERT_THAT(isThreadImpersonating(), Eq(false));
    auto token = getThreadToken();
    ASSERT_THAT(token.get(), ::testing::NotNull());
    ASSERT_THAT(isThreadImpersonating(), Eq(true));
}
