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
}