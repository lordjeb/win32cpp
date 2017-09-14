#pragma once
#include "handle.hpp"

namespace win32cpp
{
	class module_info
	{
	public:
		module_info() = delete;

		static std::wstring getModuleFilename();
		static std::wstring getModulePath();
		static std::wstring getTempPath();
	};
}
