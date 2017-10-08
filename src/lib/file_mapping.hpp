#pragma once
#include "debug.hpp"
#include <Windows.h>

namespace win32cpp
{
	class IFileMapping
	{
	public:
		virtual auto CloseHandle(_In_ HANDLE hObject) -> BOOL = 0;

		virtual auto CreateFileMappingA(
			_In_ HANDLE hFile,
			_In_opt_ LPSECURITY_ATTRIBUTES lpFileMappingAttributes,
			_In_ DWORD flProtect,
			_In_ DWORD dwMaximumSizeHigh,
			_In_ DWORD dwMaximumSizeLow,
			_In_opt_ LPCSTR lpName
		) -> HANDLE = 0;

		virtual auto CreateFileMappingW(
			_In_ HANDLE hFile,
			_In_opt_ LPSECURITY_ATTRIBUTES lpFileMappingAttributes,
			_In_ DWORD flProtect,
			_In_ DWORD dwMaximumSizeHigh,
			_In_ DWORD dwMaximumSizeLow,
			_In_opt_ LPCWSTR lpName
		) -> HANDLE = 0;

		virtual auto MapViewOfFile(
			_In_ HANDLE hFileMappingObject,
			_In_ DWORD dwDesiredAccess,
			_In_ DWORD dwFileOffsetHigh,
			_In_ DWORD dwFileOffsetLow,
			_In_ SIZE_T dwNumberOfBytesToMap
			) -> LPVOID = 0;

		virtual auto UnmapViewOfFile(_In_ LPCVOID lpBaseAddress) -> BOOL = 0;
	};

	class FileMappingImpl : public IFileMapping
	{
	public:
		auto CloseHandle(_In_ HANDLE hObject) -> BOOL override
		{
			return ::CloseHandle(hObject);
		}

		auto CreateFileMappingA(
			_In_ HANDLE hFile,
			_In_opt_ LPSECURITY_ATTRIBUTES lpFileMappingAttributes,
			_In_ DWORD flProtect,
			_In_ DWORD dwMaximumSizeHigh,
			_In_ DWORD dwMaximumSizeLow,
			_In_opt_ LPCSTR lpName
			) -> HANDLE override
		{
			return ::CreateFileMappingA(hFile, lpFileMappingAttributes, flProtect, dwMaximumSizeHigh, dwMaximumSizeLow, lpName);
		}

		auto CreateFileMappingW(
			_In_ HANDLE hFile,
			_In_opt_ LPSECURITY_ATTRIBUTES lpFileMappingAttributes,
			_In_ DWORD flProtect,
			_In_ DWORD dwMaximumSizeHigh,
			_In_ DWORD dwMaximumSizeLow,
			_In_opt_ LPCWSTR lpName
			) -> HANDLE override
		{
			return ::CreateFileMappingW(hFile, lpFileMappingAttributes, flProtect, dwMaximumSizeHigh, dwMaximumSizeLow, lpName);
		}

		auto MapViewOfFile(
			_In_ HANDLE hFileMappingObject,
			_In_ DWORD dwDesiredAccess,
			_In_ DWORD dwFileOffsetHigh,
			_In_ DWORD dwFileOffsetLow,
			_In_ SIZE_T dwNumberOfBytesToMap
			) -> LPVOID override
		{
			return ::MapViewOfFile(hFileMappingObject, dwDesiredAccess, dwFileOffsetHigh, dwFileOffsetLow, dwNumberOfBytesToMap);
		}

		auto UnmapViewOfFile(_In_ LPCVOID lpBaseAddress) -> BOOL override
		{
			return ::UnmapViewOfFile(lpBaseAddress);
		}
	};

	class map_view_deleter
	{
	public:
		typedef void* pointer;

		explicit map_view_deleter(IFileMapping* file_mapping)
			: m_file_mapping{ file_mapping }
		{
		}

		auto operator()( pointer value ) const throw( ) -> void
		{
			VERIFY( m_file_mapping->UnmapViewOfFile( value ) );
		}

	private:
		IFileMapping* m_file_mapping;
	};
}
