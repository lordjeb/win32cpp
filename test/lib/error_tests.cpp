#include "pch.h"
#include "error.h"

using namespace win32cpp;
using testing::StrEq;

TEST(error_test, getErrorMessage_converts_win32_code_to_error_message)
{
    ASSERT_THAT(getErrorMessage(ERROR_FILE_NOT_FOUND), StrEq(L"The system cannot find the file specified."));
}

TEST(error_test, check_bool)
{
    EXPECT_THROW(CHECK_BOOL(false), check_failed);
    EXPECT_NO_THROW(CHECK_BOOL(true));
}

TEST(error_test, check_count)
{
    EXPECT_THROW(CHECK_COUNT(0), check_failed);
    EXPECT_NO_THROW(CHECK_COUNT(1));
}

TEST(error_test, check_hr)
{
    EXPECT_THROW(CHECK_HR(E_FAIL), check_failed);
    EXPECT_NO_THROW(CHECK_HR(S_OK));
}

TEST(error_test, check_win32)
{
    EXPECT_THROW(CHECK_WIN32(ERROR_FILE_NOT_FOUND), check_failed);
    EXPECT_NO_THROW(CHECK_WIN32(ERROR_SUCCESS));
}

TEST(error_test, check_equal)
{
    EXPECT_THROW(CHECK_EQ(0, 1), check_failed);
    EXPECT_NO_THROW(CHECK_EQ(0, 0));
}
