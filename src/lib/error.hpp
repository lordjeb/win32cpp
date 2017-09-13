#pragma once
#include <Windows.h>
#include "heap_ptr.hpp"

#define CHECK_BOOL(br) checkBool((br), __WFILE__, __LINE__)
#define CHECK_COUNT(cnt) checkCount((cnt), __WFILE__, __LINE__)
#define CHECK_HR(dwr) checkHr((dwr), __WFILE__, __LINE__)
#define CHECK_WIN32(dwr) checkWin32((dwr), __WFILE__, __LINE__)
#define CHECK(r1, r2) check((r1), (r2), __WFILE__, __LINE__)

#define LANGID_ENGLISH 1033

namespace vxshared
{
	struct check_failed
	{
		explicit check_failed(long result) : error(result)
		{
		}

		long error;
	};

	inline void checkBool(BOOL br)
	{
		if (FALSE == br)
		{
			throw check_failed(GetLastError());
		}
	}

	inline void checkBool(BOOL br, const std::wstring& file, int line)
	{
		if (FALSE == br)
		{
			auto gle = GetLastError();
			outputDebugStringEx(L"%s(%d): BOOL result failure (%d)\n", file.c_str(), line, gle);
			throw check_failed(gle);
		}
	}

	inline void checkBool(bool br)
	{
		if (false == br)
		{
			throw check_failed(GetLastError());
		}
	}

	inline void checkBool(bool br, const std::wstring& file, int line)
	{
		if (false == br)
		{
			auto gle = GetLastError();
			outputDebugStringEx(L"%s(%d): bool result failure (%d)\n", file.c_str(), line, gle);
			throw check_failed(gle);
		}
	}

	inline void checkCount(int c)
	{
		if (0 == c)
		{
			throw check_failed(GetLastError());
		}
	}

	inline void checkCount(int c, const std::wstring& file, int line)
	{
		if (0 == c)
		{
			auto gle = GetLastError();
			outputDebugStringEx(L"%s(%d): count result failure (%d)\n", file.c_str(), line, gle);
			throw check_failed(gle);
		}
	}

	inline void checkHr(HRESULT hr)
	{
		if (S_OK != hr)
		{
			throw check_failed(hr);
		}
	}

	inline void checkHr(HRESULT hr, const std::wstring& file, int line)
	{
		if (S_OK != hr)
		{
			outputDebugStringEx(L"%s(%d): HRESULT result failure (0x%08x)\n", file.c_str(), line, hr);
			throw check_failed(hr);
		}
	}

	inline void checkWin32(DWORD dwr)
	{
		if (ERROR_SUCCESS != dwr)
		{
			throw check_failed(dwr);
		}
	}

	inline void checkWin32(DWORD dwr, const std::wstring& file, int line)
	{
		if (ERROR_SUCCESS != dwr)
		{
			outputDebugStringEx(L"%s(%d): WIN32 result failure (0x%08x)\n", file.c_str(), line, dwr);
			throw check_failed(dwr);
		}
	}

	template <typename T>
	void check(T EXPECT_TRUEed, T actual)
	{
		if (EXPECT_TRUEed != actual)
		{
			throw check_failed(0);
		}
	}

	template <typename T>
	void check(T EXPECT_TRUEed, T actual, const std::wstring& file, int line)
	{
		if (EXPECT_TRUEed != actual)
		{
			outputDebugStringEx(L"%s(%d): check failure\n", file.c_str(), line);
			throw check_failed(0);
		}
	}

	std::wstring getErrorMessage(DWORD dw, LANGID languageId = LANGID_ENGLISH);
}
