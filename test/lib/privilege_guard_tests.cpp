#include "pch.h"
#include "error.h"
#include "privilege_guard.h"
#include "module_info.h"
#include <Windows.h>

using namespace win32cpp;
using ::testing::Eq;
using ::testing::Throws;

struct privilege_guard_test : public ::testing::Test
{
    unique_token_handle threadToken = getThreadToken();
};

TEST_F(privilege_guard_test, acquires_disabled_privilege)
{
    auto shutdown_privilege = privilege_guard{ threadToken.get(), SE_SHUTDOWN_NAME };   // Disabled by default
    ASSERT_THAT(shutdown_privilege.enabled(), Eq(true));
}

TEST_F(privilege_guard_test, doesnt_acquire_enabled_privilege)
{
    auto change_notify_privilege = privilege_guard{ threadToken.get(), SE_CHANGE_NOTIFY_NAME };   // Enabled by default
    ASSERT_THAT(change_notify_privilege.enabled(), Eq(false));
}

TEST_F(privilege_guard_test, throws_on_denied_privilege)
{
    auto fn = [=]() { privilege_guard x{ threadToken.get(), SE_TCB_NAME }; };
    ASSERT_THAT([&]() { fn(); }, Throws<check_failed>());
}
