#pragma once
#include "handle.hpp"

namespace win32cpp
{
	std::wstring getModuleFilename();
	std::wstring getModulePath();
	std::wstring getTempPath();
}
