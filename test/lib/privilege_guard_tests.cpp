#include "pch.h"
#include "error.h"
#include "privilege_guard.h"
#include "module_info.h"
#include <Windows.h>

struct privilege_guard_test : public Test
{
    unique_token_handle threadToken = getThreadToken();
};

TEST_F(privilege_guard_test, acquires_disabled_privilege)
{
    auto shutdown_privilege = privilege_guard{ threadToken.get(), SE_SHUTDOWN_NAME };   // Disabled by default
    ASSERT_TRUE(shutdown_privilege.enabled());
}

TEST_F(privilege_guard_test, doesnt_acquire_enabled_privilege)
{
    auto change_notify_privilege = privilege_guard{ threadToken.get(), SE_CHANGE_NOTIFY_NAME };   // Enabled by default
    ASSERT_FALSE(change_notify_privilege.enabled());
}

TEST_F(privilege_guard_test, throws_on_denied_privilege)
{
    auto fn = [=]() { privilege_guard x{ threadToken.get(), SE_TCB_NAME }; };
    ASSERT_THROW(fn(), check_failed);
}
