#pragma once
#include <memory>

namespace win32cpp
{
	struct heap_ptr_deleter
	{
		void operator()(void *p) const
		{
			HeapFree(GetProcessHeap(), 0, p);
		}
	};

	typedef std::unique_ptr<void, heap_ptr_deleter> heap_ptr;

	struct local_ptr_deleter
	{
		void operator()(void *p) const
		{
			LocalFree(p);
		}
	};

	typedef std::unique_ptr<void, local_ptr_deleter> local_ptr;

	struct sid_ptr_deleter
	{
		void operator()(void *p) const
		{
			FreeSid(p);
		}
	};

	typedef std::unique_ptr<void, sid_ptr_deleter> sid_ptr;
}