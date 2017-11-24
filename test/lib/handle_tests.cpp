#include "pch.h"
#include "handle.h"

using namespace win32cpp;
using testing::IsNull;
using testing::Eq;

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
	typedef HANDLE pointer;
	static auto invalid() throw() -> pointer
	{
		return INVALID_HANDLE_VALUE;
	}
};

template <typename Traits>
struct mock_handle_closer_state
{
	typedef typename Traits::pointer pointer;

	bool m_closed{ false };
	pointer m_closed_value{ Traits::invalid() };
};

template <typename Traits>
class mock_handle_closer
{
protected:
	typedef typename Traits::pointer pointer;

public:
	explicit mock_handle_closer(mock_handle_closer_state<Traits>* state)
		: m_state{ state }
	{
	}

	mock_handle_closer(const mock_handle_closer& src)
		: m_state{ src.m_state }
	{
	}

	mock_handle_closer& operator=(const mock_handle_closer& src)
	{
		m_state = src.m_state;
		return *this;
	}

	auto operator()(pointer value) -> void
	{
		m_state->m_closed = true;
		m_state->m_closed_value = value;
	}

private:
	mock_handle_closer_state<Traits>* m_state;
};

typedef basic_unique_handle<mock_handle_traits, mock_handle_closer<mock_handle_traits>> mock_handle;

struct basic_unique_handle_test : public ::testing::Test
{
	mock_handle_closer_state<mock_handle_traits> mock_closer_state;
	mock_handle_closer<mock_handle_traits> mock_closer{ &mock_closer_state };
	const HANDLE fake_handle_value = (HANDLE) 0xBAADF00DLL;
};

TEST_F(basic_unique_handle_test, is_noncopyable_and_moveable)
{
	ASSERT_FALSE(std::is_copy_constructible<mock_handle>::value);
	ASSERT_FALSE(std::is_copy_assignable<mock_handle>::value);
	ASSERT_TRUE(std::is_move_constructible<mock_handle>::value);
	ASSERT_TRUE(std::is_move_assignable<mock_handle>::value);
}

TEST_F(basic_unique_handle_test, closes_handle)
{
	{
		mock_handle test_handle{ mock_closer, fake_handle_value };
	}
	ASSERT_TRUE(mock_closer_state.m_closed);
	ASSERT_THAT(mock_closer_state.m_closed_value, Eq(fake_handle_value));
}

TEST_F(basic_unique_handle_test, doesnt_close_invalid_handle)
{
	{
		mock_handle test_handle{ mock_closer };
	}
	ASSERT_FALSE(mock_closer_state.m_closed);
}

TEST_F(basic_unique_handle_test, closes_handle_when_reset)
{
	mock_handle test_handle{ mock_closer, fake_handle_value };
	test_handle.reset(NULL);
	ASSERT_TRUE(mock_closer_state.m_closed);
	ASSERT_THAT(mock_closer_state.m_closed_value, Eq(fake_handle_value));
	ASSERT_THAT(test_handle.get(), IsNull());
}

TEST_F(basic_unique_handle_test, doesnt_close_released_handle)
{
	{
		mock_handle test_handle{ mock_closer, fake_handle_value };
		auto local_handle = test_handle.release();
	}
	ASSERT_FALSE(mock_closer_state.m_closed);
}
