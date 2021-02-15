#include "pch.h"
#include "error.h"
#include "string_extensions.h"

using namespace win32cpp;
using testing::StrEq;

TEST(error_test, getErrorMessage_converts_win32_code_to_error_message)
{
    ASSERT_THAT(getErrorMessage(ERROR_FILE_NOT_FOUND), StrEq(L"The system cannot find the file specified."));
}

TEST(error_test, getErrorMessage_language_fallback)
{
    LANGID frenchStandard = 1036;
    ASSERT_THAT(getErrorMessage(ERROR_FILE_NOT_FOUND, frenchStandard),
                StrEq(L"The system cannot find the file specified."));
}

TEST(error_test, getErrorMessage_language_fallback_disallowed)
{
    LANGID frenchStandard = 1036;
    EXPECT_THROW(getErrorMessage(ERROR_FILE_NOT_FOUND, frenchStandard, false), win32_check_failed);
}

TEST(error_test, check_bool)
{
    EXPECT_THROW(CHECK_BOOL(false), win32_check_failed);
    EXPECT_NO_THROW(CHECK_BOOL(true));
}

TEST(error_test, check_count)
{
    EXPECT_THROW(CHECK_COUNT(0), win32_check_failed);
    EXPECT_NO_THROW(CHECK_COUNT(1));
}

TEST(error_test, check_hr)
{
    EXPECT_THROW(CHECK_HR(E_FAIL), hresult_check_failed);
    EXPECT_NO_THROW(CHECK_HR(S_OK));
}

TEST(error_test, check_win32)
{
    EXPECT_THROW(CHECK_WIN32(ERROR_FILE_NOT_FOUND), win32_check_failed);
    EXPECT_NO_THROW(CHECK_WIN32(ERROR_SUCCESS));
}

TEST(error_test, check_equal)
{
    EXPECT_THROW(CHECK_EQ(0, 1), check_failed);
    EXPECT_NO_THROW(CHECK_EQ(0, 0));
}

TEST(error_test, check_failed_inherits_from_std_exception)
{
    EXPECT_THROW(throw check_failed(0), std::exception);
}

TEST(error_test, check_failed_converts_message_to_utf8)
{
    constexpr auto message = L"Something unexpected happened అఃఓఅౌ౩";
    try
    {
        throw check_failed(0, message);
    }
    catch (const check_failed& e)
    {
        ASSERT_STREQ(
            e.what(),
            "[0x00000000] Something unexpected happened \xE0\xB0\x85\xE0\xB0\x83\xE0\xB0\x93\xE0\xB0\x85\xE0\xB1\x8C\xE0\xB1\xA9");
        ASSERT_STREQ(e.w_what().c_str(), format(L"[0x00000000] %s", message).c_str());
    }
}

TEST(error_test, check_macro_accepts_optional_message)
{
    int lineNum;
    try
    {
        // clang-format off
        // The following must be on the same line for the string comparison below to work correctly
        lineNum = __LINE__; CHECK_HR(E_FAIL, L"The COM component gave up");
        // clang-format on
    }
    catch (const hresult_check_failed& e)
    {
        ASSERT_STREQ(e.what(),
                     format("error_tests.cpp(%d): [HRESULT: 0x80004005] The COM component gave up", lineNum).c_str());
    }
}

TEST(error_test, hresult_check_failed_contains_hresult)
{
    try
    {
        CHECK_HR(E_FAIL);
    }
    catch (const hresult_check_failed& e)
    {
        ASSERT_EQ(e.hresult(), E_FAIL);
        ASSERT_EQ(e.hresult(), e.error);
    }
}

TEST(error_test, win32_check_failed_contains_wcode)
{
    try
    {
        CHECK_WIN32(ERROR_FILE_NOT_FOUND);
    }
    catch (const win32_check_failed& e)
    {
        ASSERT_EQ(e.wcode(), static_cast<DWORD>(ERROR_FILE_NOT_FOUND));
        ASSERT_EQ(e.wcode(), static_cast<DWORD>(e.error));
    }
}

TEST(error_test, win32_check_failed_gives_custom_what)
{
    int lineNum;
    try
    {
        // clang-format off
        // The following must be on the same line for the string comparison below to work correctly
        lineNum = __LINE__; CHECK_WIN32(ERROR_BAD_ARGUMENTS, L"Bad parameter");
        // clang-format on
    }
    catch (const std::exception& e)
    {
        ASSERT_STREQ(e.what(), format("error_tests.cpp(%d): [WCODE: 0x000000a0] Bad parameter", lineNum).c_str());
    }
}

TEST(error_test, hresult_check_failed_gives_custom_what)
{
    int lineNum;
    try
    {
        // clang-format off
        // The following must be on the same line for the string comparison below to work correctly
        lineNum = __LINE__; CHECK_HR(E_INVALIDARG, L"Bad parameter");
        // clang-format on
    }
    catch (const std::exception& e)
    {
        ASSERT_STREQ(e.what(), format("error_tests.cpp(%d): [HRESULT: 0x80070057] Bad parameter", lineNum).c_str());
    }
}

TEST(error_test, win32_check_failed_without_macro_gives_what_without_file_and_line)
{
    try
    {
        throw win32_check_failed(ERROR_BAD_ARGUMENTS, L"Bad parameter");
    }
    catch (const std::exception& e)
    {
        ASSERT_STREQ(e.what(), format("[WCODE: 0x000000a0] Bad parameter").c_str());
    }
}

TEST(error_test, win32_check_failed_without_line_gives_what_without_line)
{
    try
    {
        throw win32_check_failed(ERROR_BAD_ARGUMENTS, __FILEW__, -1, L"Bad parameter");
    }
    catch (const std::exception& e)
    {
        ASSERT_STREQ(e.what(), format("error_tests.cpp: [WCODE: 0x000000a0] Bad parameter").c_str());
    }
}
