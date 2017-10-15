#pragma once
#include "debug.hpp"
#include <Windows.h>
#include <ioc\file_mapping.h>

namespace win32cpp
{
	class map_view_deleter
	{
	public:
		explicit map_view_deleter(IFileMapping* file_mapping)
			: m_file_mapping{ file_mapping }
		{
		}

		auto operator()(void* value) const throw( ) -> void
		{
			VERIFY( m_file_mapping->UnmapViewOfFile( value ) );
		}

	private:
		IFileMapping* m_file_mapping;
	};
}
