#include "dynamic_library.h"
#include <Windows.h>
#include <gmock/gmock.h>

using namespace win32cpp;
using ::testing::Eq;

TEST(dynamic_library_test, load_and_call_Sleep)
{
    DynamicFunction<VOID, DWORD> Sleep("kernel32.dll", "Sleep");
    Sleep(10);
}

TEST(dynamic_library_test, load_and_call_RtlInitUnicodeString)
{
    typedef struct
    {
        USHORT Length;
        USHORT MaximumLength;
        PWSTR Buffer;
    } UNICODE_STRING, *PUNICODE_STRING;

    DynamicFunction<VOID, PUNICODE_STRING, PCWSTR> RtlInitUnicodeString("ntdll.dll", "RtlInitUnicodeString");

    UNICODE_STRING unicode_string;
    RtlInitUnicodeString(&unicode_string, L"TestString");

    ASSERT_THAT(unicode_string.Length, Eq(20));
}

TEST(dynamic_library_test, unloadable_library_throws)
{
    // Note: compiler issues prevent us from converting this to ASSERT_THAT(..., Throws)
    DynamicFunction<VOID, DWORD> Sleep("blah.dll", "Sleep");
    ASSERT_THROW(Sleep(10), check_failed);
}

TEST(dynamic_library_test, unloadable_function_throws)
{
    // Note: compiler issues prevent us from converting this to ASSERT_THAT(..., Throws)
    DynamicFunction<VOID, DWORD> Sleep("kernel32.dll", "Blah");
    ASSERT_THROW(Sleep(10), check_failed);
}
