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
	constexpr wchar_t* message = L"Something unexpected happened అఃఓఅౌ౩";
	try
	{
		throw check_failed(0, message);
	}
	catch (const check_failed& e)
	{
		ASSERT_STREQ(e.what(), "Something unexpected happened \xE0\xB0\x85\xE0\xB0\x83\xE0\xB0\x93\xE0\xB0\x85\xE0\xB1\x8C\xE0\xB1\xA9");
		ASSERT_STREQ(e.w_what().c_str(), message);
	}
}

TEST(error_test, check_macro_accepts_optional_message)
{
	try
	{
		CHECK_HR(E_FAIL, L"The COM component gave up");
	}
	catch (const hresult_check_failed& e)
	{
		ASSERT_STREQ(e.what(), "The COM component gave up");
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
		ASSERT_EQ(e.wcode(), ERROR_FILE_NOT_FOUND);
		ASSERT_EQ(e.wcode(), e.error);
	}
}
