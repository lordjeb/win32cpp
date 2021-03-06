#include "error.h"
#include <gmock/gmock.h>
#include "string_extensions.h"

using namespace win32cpp;
using ::testing::EndsWith;
using ::testing::Eq;
using ::testing::HasSubstr;
using ::testing::StrEq;
using ::testing::Throws;
using ::testing::ThrowsMessage;

static_assert(std::is_base_of<std::exception, win32cpp::check_failed>::value,
              "win32cpp::check_failed should be an std::exception-derived type");

static_assert(std::is_base_of<std::exception, win32cpp::win32_check_failed>::value,
              "win32cpp::win32_check_failed should be an std::exception-derived type");

static_assert(std::is_base_of<std::exception, win32cpp::hresult_check_failed>::value,
              "win32cpp::hresult_check_failed should be an std::exception-derived type");

TEST(error_test, getErrorMessage_converts_win32_code_to_error_message)
{
    ASSERT_THAT(getErrorMessage(ERROR_FILE_NOT_FOUND), StrEq(L"The system cannot find the file specified."));
}

TEST(error_test, getErrorMessage_language_fallback)
{
    LANGID langId{ 1036 };
    ASSERT_THAT(getErrorMessage(ERROR_FILE_NOT_FOUND, langId), StrEq(L"The system cannot find the file specified."));
}

TEST(error_test, getErrorMessage_throws_when_language_fallback_disallowed)
{
    LANGID langId{ 1036 };
    ASSERT_THAT(1033, Eq(GetUserDefaultLangID()));
    ASSERT_THAT([langId]() { getErrorMessage(ERROR_FILE_NOT_FOUND, langId, false); },
                ThrowsMessage<win32_check_failed>(HasSubstr("[WCODE: 0x")));
}

TEST(error_test, getErrorMessage_with_default_language)
{
    ASSERT_THAT(1033, Eq(GetUserDefaultLangID()));
    ASSERT_THAT(getErrorMessage(ERROR_FILE_NOT_FOUND), StrEq(L"The system cannot find the file specified."));
}

TEST(error_test, check_bool_negative)
{
    ASSERT_THAT([]() { CHECK_BOOL(false); }, Throws<win32_check_failed>());
}

TEST(error_test, check_bool_positive)
{
    ASSERT_NO_THROW(CHECK_BOOL(true));
}

TEST(error_test, check_count_negative)
{
    ASSERT_THAT([]() { CHECK_COUNT(0); }, Throws<win32_check_failed>());
}

TEST(error_test, check_count_positive)
{
    ASSERT_NO_THROW(CHECK_COUNT(1));
}

TEST(error_test, check_hr_negative)
{
    ASSERT_THAT([]() { CHECK_HR(E_FAIL); }, Throws<hresult_check_failed>());
}

TEST(error_test, check_hr_positive)
{
    ASSERT_NO_THROW(CHECK_HR(S_OK));
}

TEST(error_test, check_win32_negative)
{
    ASSERT_THAT([]() { CHECK_WIN32(ERROR_FILE_NOT_FOUND); }, Throws<win32_check_failed>());
}

TEST(error_test, check_win32_positive)
{
    ASSERT_NO_THROW(CHECK_WIN32(ERROR_SUCCESS));
}

TEST(error_test, check_equal_negative)
{
    ASSERT_THAT([]() { CHECK_EQ(0, 1); }, Throws<check_failed>());
}

TEST(error_test, check_equal_positive)
{
    ASSERT_NO_THROW(CHECK_EQ(0, 0));
}

TEST(error_test, check_failed_converts_message_to_utf8)
{
    constexpr auto message = L"Something unexpected happened అఃఓఅౌ౩";
    ASSERT_THAT(
        [message]() { throw check_failed(0, message); },
        ThrowsMessage<check_failed>(
            "[0x00000000] Something unexpected happened \xE0\xB0\x85\xE0\xB0\x83\xE0\xB0\x93\xE0\xB0\x85\xE0\xB1\x8C\xE0\xB1\xA9"));
}

TEST(error_test, check_failed_converts_message_to_wstr)
{
    constexpr auto message = L"Something unexpected happened అఃఓఅౌ౩";
    try
    {
        throw check_failed(0, message);
    }
    catch (const check_failed& e)
    {
        ASSERT_THAT(e.w_what(), StrEq(format(L"[0x00000000] %s", message)));
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
        ASSERT_THAT(e.what(),
                    StrEq(format("error_tests.cpp(%d): [HRESULT: 0x80004005] The COM component gave up", lineNum)));
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
        ASSERT_THAT(e.hresult(), Eq(E_FAIL));
        ASSERT_THAT(e.hresult(), Eq(e.error));
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
        ASSERT_THAT(e.wcode(), Eq(static_cast<DWORD>(ERROR_FILE_NOT_FOUND)));
        ASSERT_THAT(e.wcode(), Eq(static_cast<DWORD>(e.error)));
    }
}

TEST(error_test, win32_check_failed_gives_custom_what)
{
    // Note: Cannot use ASSERT_THAT(.., Throws) because of the order of evaluation of the arguments resulting
    // in the lineNumber not being updated for the matcher.
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
        ASSERT_THAT(e.what(), StrEq(format("error_tests.cpp(%d): [WCODE: 0x000000a0] Bad parameter", lineNum)));
    }
}

TEST(error_test, hresult_check_failed_gives_custom_what)
{
    // Note: Cannot use ASSERT_THAT(.., Throws) because of the order of evaluation of the arguments resulting
    // in the lineNumber not being updated for the matcher.
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
        ASSERT_THAT(e.what(), StrEq(format("error_tests.cpp(%d): [HRESULT: 0x80070057] Bad parameter", lineNum)));
    }
}

TEST(error_test, win32_check_failed_without_macro_gives_what_without_file_and_line)
{
    ASSERT_THAT([]() { throw win32_check_failed(ERROR_BAD_ARGUMENTS, L"Bad parameter"); },
                ThrowsMessage<std::exception>("[WCODE: 0x000000a0] Bad parameter"));
}

TEST(error_test, win32_check_failed_without_line_gives_what_without_line)
{
    ASSERT_THAT([]() { throw win32_check_failed(ERROR_BAD_ARGUMENTS, __FILEW__, -1, L"Bad parameter"); },
                ThrowsMessage<std::exception>("error_tests.cpp: [WCODE: 0x000000a0] Bad parameter"));
}

TEST(error_test, check_failed_with_unknown_error_message)
{
    ASSERT_THAT([]() { CHECK_HR(0x8e5e04c0); },
                ThrowsMessage<hresult_check_failed>(EndsWith("[HRESULT: 0x8e5e04c0] ")));
}
