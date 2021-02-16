#include "pch.h"
#include "memory.h"
#include <Windows.h>
#include <sddl.h>

using namespace win32cpp;
using ::testing::NotNull;
using ::testing::StartsWith;

TEST(memory_test, heap_ptr)
{
    auto hptr = std::unique_ptr<BYTE, heap_ptr_deleter>{ (BYTE*)::HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 256) };
    ASSERT_THAT(hptr.get(), NotNull());
}

TEST(memory_test, local_ptr)
{
    auto lptr = std::unique_ptr<BYTE, local_ptr_deleter>{ (BYTE*)::LocalAlloc(LPTR, 256) };
    ASSERT_THAT(lptr.get(), NotNull());
}

TEST(memory_test, global_ptr)
{
    auto gptr = std::unique_ptr<BYTE, global_ptr_deleter>{ (BYTE*)::GlobalAlloc(GPTR, 256) };
    ASSERT_THAT(gptr.get(), NotNull());
}

TEST(memory_test, sid_ptr)
{
    auto pSid = PSID{};
    auto sidIdentifierAuthority = SID_IDENTIFIER_AUTHORITY{ SECURITY_NT_AUTHORITY };
    CHECK_BOOL(
        ::AllocateAndInitializeSid(&sidIdentifierAuthority, 1, DOMAIN_USER_RID_ADMIN, 0, 0, 0, 0, 0, 0, 0, &pSid));
    auto sid = std::unique_ptr<void, sid_ptr_deleter>{ pSid };

    auto pStringSid = LPTSTR{};
    CHECK_BOOL(::ConvertSidToStringSid(sid.get(), &pStringSid));
    auto stringSid = std::unique_ptr<wchar_t, local_ptr_deleter>{ pStringSid };

    ASSERT_THAT(stringSid.get(), StartsWith(L"S-1-"));
}

TEST(memory_test, ptr_setter)
{
    auto pSid = PSID{};
    auto sidIdentifierAuthority = SID_IDENTIFIER_AUTHORITY{ SECURITY_NT_AUTHORITY };
    CHECK_BOOL(
        ::AllocateAndInitializeSid(&sidIdentifierAuthority, 1, DOMAIN_USER_RID_ADMIN, 0, 0, 0, 0, 0, 0, 0, &pSid));
    auto sid = std::unique_ptr<void, sid_ptr_deleter>{ pSid };

    auto stringSid = std::unique_ptr<wchar_t, local_ptr_deleter>{};
    CHECK_BOOL(::ConvertSidToStringSid(sid.get(), &ptr_setter(stringSid)));

    ASSERT_THAT(stringSid.get(), StartsWith(L"S-1-"));
}
