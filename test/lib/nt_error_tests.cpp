#include "pch.h"
#include "string_extensions.h"

typedef _Return_type_success_(return >= 0) LONG NTSTATUS;
#include "nt_error.h"

using namespace win32cpp;

TEST(nt_error_test, check_ntstatus)
{
	EXPECT_THROW(CHECK_NTSTATUS(-1), ntstatus_check_failed);
	EXPECT_NO_THROW(CHECK_NTSTATUS(0));
}

TEST(nt_error_test, ntstatus_check_failed_contains_ntstatus)
{
	try
	{
		CHECK_NTSTATUS(-1);
	}
	catch (const ntstatus_check_failed& e)
	{
		ASSERT_EQ(e.ntstatus(), -1);
		ASSERT_EQ(e.ntstatus(), e.error);
	}
}

TEST(nt_error_test, ntstatus_check_failed_gives_custom_what)
{
	int lineNum;
	try
	{
		lineNum = __LINE__; CHECK_NTSTATUS(STATUS_INVALID_PARAMETER, L"Bad parameter");
	}
	catch (const std::exception& e)
	{
		ASSERT_STREQ(e.what(), format("nt_error_tests.cpp(%d): [NTSTATUS: 0xc000000d] Bad parameter", lineNum).c_str());
	}
}
