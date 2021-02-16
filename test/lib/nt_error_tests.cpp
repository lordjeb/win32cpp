#include "pch.h"
#include "string_extensions.h"

typedef _Return_type_success_(return >= 0) LONG NTSTATUS;
#include "nt_error.h"

using namespace win32cpp;
using ::testing::Eq;
using ::testing::StrEq;
using ::testing::Throws;

TEST(nt_error_test, check_ntstatus_positive)
{
    ASSERT_THAT([]() { CHECK_NTSTATUS(-1); }, Throws<ntstatus_check_failed>());
}

TEST(nt_error_test, check_ntstatus_negative)
{
    ASSERT_NO_THROW(CHECK_NTSTATUS(0));
}

TEST(nt_error_test, ntstatus_check_failed_contains_ntstatus)
{
    try
    {
        CHECK_NTSTATUS(-1);
    }
    catch (const ntstatus_check_failed& e)
    {
        ASSERT_THAT(e.ntstatus(), Eq(-1));
        ASSERT_THAT(e.ntstatus(), Eq(e.error));
    }
}

TEST(nt_error_test, ntstatus_check_failed_gives_custom_what)
{
    int lineNum;
    try
    {
        // clang-format off
        // The following must be on the same line for the string comparison below to work correctly
        lineNum = __LINE__; CHECK_NTSTATUS(STATUS_INVALID_PARAMETER, L"Bad parameter");
        // clang-format on        
    }
    catch (const std::exception& e)
    {
        ASSERT_THAT(e.what(), StrEq(format("nt_error_tests.cpp(%d): [NTSTATUS: 0xc000000d] Bad parameter", lineNum)));
    }
}
