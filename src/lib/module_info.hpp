#pragma once
#include "stdafx.h"
#include "handle.hpp"

namespace win32cpp
{
	class Imodule_info
	{
	public:
		virtual void enablePrivilege(HANDLE tokenHandle, const std::wstring& privilege, bool enable) const = 0;
		virtual unique_handle getCurrentToken() const = 0;
		virtual std::wstring getModuleFilename() const = 0;
		virtual std::wstring getModulePath() const = 0;
	};

	class module_info : public Imodule_info
	{
	public:
		explicit module_info();
		virtual ~module_info();

		module_info(const module_info&) = delete;
		module_info& operator=(const module_info&) = delete;

		void enablePrivilege(HANDLE tokenHandle, const std::wstring& privilege, bool enable) const override;
		unique_handle getCurrentToken() const override;
		std::wstring getModuleFilename() const override;
		std::wstring getModulePath() const override;
	};
}
