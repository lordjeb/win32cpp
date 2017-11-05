#pragma once
#include <Windows.h>

namespace win32cpp
{
	inline namespace ioc
	{
		class IHandle
		{
		public:
			virtual auto CloseHandle(_In_ HANDLE hObject)->BOOL = 0;
		};

		class HandleImpl : public IHandle
		{
		public:
			virtual auto CloseHandle(_In_ HANDLE hObject)->BOOL
			{
				return ::CloseHandle(hObject);
			}
		};
	}
}
