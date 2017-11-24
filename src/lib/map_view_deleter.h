#pragma once
#include "debug.h"
#include <Windows.h>
#include <win32api_abstractions.h>

namespace win32cpp
{
	class map_view_deleter
	{
	public:
		explicit map_view_deleter(std::shared_ptr<IFile> pFile)
			: m_pFile{ pFile }
		{
		}

		auto operator()(void* value) const throw( ) -> void
		{
			VERIFY( m_pFile->UnmapViewOfFile( value ) );
		}

	private:
		std::shared_ptr<IFile> m_pFile;
	};
}
