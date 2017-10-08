#pragma once
#include "debug.hpp"
#include <Windows.h>

namespace win32cpp
{
	struct map_view_deleter
	{
		typedef unsigned char* pointer;
		auto operator()( pointer value ) const throw( ) -> void
		{
			VERIFY( UnmapViewOfFile( value ) );
		}
	};
}
