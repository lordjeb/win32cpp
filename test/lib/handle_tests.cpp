#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include "handle.h"

using namespace win32cpp;

// The following monstrosity of code is required in order to mock something that has static methods.
// The basic_unique_handle needs a traits class with a static invalid() and close() method. gmock
// needs to operate on non-static methods. So in each test, we have a mock_handle_traits that is
// setup with gmock. Then there is a static mock_static_handle_traits that is provided to the
// basic_unique_handle. In between, we have a global mock_handle_traits_bridge that the static
// traits class can call to pass the calls through to the actual mock.
//
// We hate this, but we don't see another way to test this without avoiding the STL pattern of
// the basic_ type with injected traits.

struct mock_handle_traits
{
	MOCK_METHOD0(invalid, HANDLE());
	MOCK_METHOD1(close, BOOL(HANDLE));
};

struct mock_handle_traits_bridge
{
	mock_handle_traits* m_ptr;

	typedef HANDLE pointer;

	auto invalid() throw() -> pointer
	{
		return m_ptr->invalid();
	}

	auto close(pointer value) throw() -> void
	{
		m_ptr->close(value);
	}
};
mock_handle_traits_bridge handle_traits_bridge;

struct mock_static_handle_traits
{
	typedef HANDLE pointer;

	static auto invalid() throw() -> pointer
	{
		return handle_traits_bridge.invalid();
	}

	static auto close(pointer value) throw() -> void
	{
		handle_traits_bridge.close(value);
	}
};

struct basic_unique_handle_test : public ::testing::Test
{
	basic_unique_handle_test()
	{
		handle_traits_bridge.m_ptr = &mock;
	}

	mock_handle_traits mock;
};

TEST(basic_unique_handle, is_noncopyable_and_moveable)
{
	ASSERT_FALSE(std::is_copy_constructible<basic_unique_handle<mock_static_handle_traits>>::value);
	ASSERT_FALSE(std::is_copy_assignable<basic_unique_handle<mock_static_handle_traits>>::value);
	ASSERT_TRUE(std::is_move_constructible<basic_unique_handle<mock_static_handle_traits>>::value);
	ASSERT_TRUE(std::is_move_assignable<basic_unique_handle<mock_static_handle_traits>>::value);
}

TEST_F(basic_unique_handle_test, closes_handle)
{
	EXPECT_CALL(mock, close((HANDLE)42))
		.Times(1);

	basic_unique_handle<mock_static_handle_traits> test_handle{ (HANDLE)42 };
}

TEST_F(basic_unique_handle_test, doesnt_close_invalid_handle)
{
	basic_unique_handle<mock_static_handle_traits> test_handle{};
}

TEST_F(basic_unique_handle_test, closes_handle_when_reset)
{
	EXPECT_CALL(mock, close((HANDLE) 42))
		.Times(1);

	basic_unique_handle<mock_static_handle_traits> test_handle{ (HANDLE) 42 };
	test_handle.reset(NULL);

	ASSERT_EQ(NULL, test_handle.get());
}

TEST_F(basic_unique_handle_test, doesnt_close_released_handle)
{
	mock_handle_traits mock;
	handle_traits_bridge.m_ptr = &mock;

	basic_unique_handle<mock_static_handle_traits> test_handle{ (HANDLE) 42 };
	auto local_handle = test_handle.release();
}
