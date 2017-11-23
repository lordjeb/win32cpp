#pragma once
#include <Windows.h>


//
//	Defines a number of constants that can be used in Win32 programming to make the code more expressive
//	and understandable. The best example is CreateEvent( NULL, TRUE, FALSE, NULL ), which becomes
//	CreateEvent( NoSecurityAttributes, ManualReset, Unsignalled, Unnammed ).
//

namespace win32cpp
{
	static const HANDLE NoTemplateFile = nullptr;

	static const LPSECURITY_ATTRIBUTES NoSecurityAttributes = nullptr;

	static const LPSTR UnnamedA = nullptr;
	static const LPWSTR UnnamedW = nullptr;
	#ifndef _UNICODE
	#define Unnamed UnnamedA
	#else
	#define Unnamed UnnamedW
	#endif

	enum { ManualReset = TRUE, AutoReset = FALSE };

	enum { Signalled = TRUE, Unsignalled = FALSE };

	enum { WaitForAllHandles = TRUE, WaitForAnyHandle = FALSE };

	enum { OpenAsProcess = TRUE, OpenAsThread = FALSE };

	enum { Enable = true, Disable = false };

	enum { InitiallyOwned = TRUE, NotInitiallyOwned = FALSE };
}
