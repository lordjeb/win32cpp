#pragma once
#include <memory>
#include <Windows.h>

namespace win32cpp
{
	struct heap_ptr_deleter
	{
		void operator()(void *p) const
		{
			HeapFree(GetProcessHeap(), 0, p);
		}
	};

	struct local_ptr_deleter
	{
		void operator()(void *p) const
		{
			LocalFree(p);
		}
	};

	struct global_ptr_deleter
	{
		void operator()(void *p) const
		{
			GlobalFree(p);
		}
	};

	struct sid_ptr_deleter
	{
		void operator()(void *p) const
		{
			FreeSid(p);
		}
	};

	template<class T>
	class ptr_setter
	{
	public:
		ptr_setter(T& Ptr) : m_Ptr{ Ptr } {}
		~ptr_setter() { m_Ptr.reset(m_RawPtr); }

		ptr_setter(const ptr_setter&) = delete;
		ptr_setter& operator=(const ptr_setter&) = delete;

		auto operator&() { return &m_RawPtr; }

	private:
		T& m_Ptr;
		typename T::pointer m_RawPtr{};
	};


	// Macro will not be needed with C++17 class template deduction.
	// If you dislike macros (as all normal people should)
	// it's possible to replace it with a helper function,
	// although this would make the code a little more complex.

	#define ptr_setter(ptr) ptr_setter<decltype(ptr)>(ptr)
}