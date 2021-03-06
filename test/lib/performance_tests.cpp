#include "performance.h"
#include <Windows.h>
#include <gmock/gmock.h>

using namespace win32cpp;
using ::testing::Ge;

TEST(performance_test, measures_performance_with_lambda)
{
    auto ms = measureElapsedTime([]() { ::Sleep(10); });
    ASSERT_THAT(ms, Ge(10));
}

auto fn() -> void
{
    ::Sleep(10);
}

TEST(performance_test, measures_performance_with_function)
{
    auto ms = measureElapsedTime(fn);
    ASSERT_THAT(ms, Ge(10));
}

class sleeper
{
    long m_ms;

public:
    explicit sleeper(long ms) : m_ms{ ms }
    {
    }

    virtual void operator()()
    {
        ::Sleep(m_ms);
    }
};

TEST(performance_test, measures_performance_with_functor)
{
    auto sl = sleeper{ 10 };
    auto ms = measureElapsedTime(sl);
    ASSERT_THAT(ms, Ge(10));
}
