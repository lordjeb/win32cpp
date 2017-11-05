#pragma once
#include <Windows.h>

namespace win32cpp
{
	inline namespace ioc
	{
		class IError
		{
		public:
			virtual auto GetLastError()->DWORD = 0;
		};

		class ErrorImpl : public IError
		{
		public:
			virtual auto GetLastError()->DWORD override;
			{
				return ::GetLastError();
			}
		};
	}
}
