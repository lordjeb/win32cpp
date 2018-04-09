#pragma once
#include "error.h"

#define CHECK_NTSTATUS(nt, ...) win32cpp::checkNtStatus((nt), __FILEW__, __LINE__, __VA_ARGS__)

// Copied from ntdef.h, we assume NTSTATUS is typedef'd if you include this file
#ifndef NT_SUCCESS
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#endif

namespace win32cpp
{
	struct ntstatus_check_failed : public check_failed
	{
		explicit ntstatus_check_failed(NTSTATUS result, const wchar_t* message = L"") : check_failed(result, message)
		{
		}

		ntstatus_check_failed(NTSTATUS result, const wchar_t* file, int line, const wchar_t* message = L"") : check_failed(result, file, line, message)
		{
		}

		virtual ~ntstatus_check_failed() {}

		NTSTATUS ntstatus() const
		{
			return error;
		}
	};

	inline void checkNtStatus(NTSTATUS ntstatus)
	{
		if (!NT_SUCCESS(ntstatus))
		{
			throw ntstatus_check_failed(ntstatus);
		}
	}

	inline void checkNtStatus(NTSTATUS ntstatus, const wchar_t* file, int line, const wchar_t* message = L"")
	{
		if (!NT_SUCCESS(ntstatus))
		{
			outputDebugStringEx(L"%s(%d): NTSTATUS result failure (0x%08x)\n", file, line, ntstatus);
			throw ntstatus_check_failed(ntstatus, file, line, message);
		}
	}
}
