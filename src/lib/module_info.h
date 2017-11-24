#pragma once
#include "handle.h"

namespace win32cpp
{
	auto getModuleFilename()->std::wstring;
	auto getModulePath()->std::wstring;
	auto getTempPath()->std::wstring;
	auto getWindowsPath()->std::wstring;
	auto getSystemPath()->std::wstring;
	//auto getThreadToken()->unique_token_handle;
}
