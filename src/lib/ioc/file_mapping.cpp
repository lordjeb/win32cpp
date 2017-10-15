#include "file_mapping.h"

namespace win32cpp
{
    inline namespace ioc
    {
        auto FileMappingImpl::CloseHandle(_In_ HANDLE hObject) -> BOOL
        {
            return ::CloseHandle(hObject);
        }
        
        auto FileMappingImpl::CreateFileMappingA(
            _In_ HANDLE hFile,
            _In_opt_ LPSECURITY_ATTRIBUTES lpFileMappingAttributes,
            _In_ DWORD flProtect,
            _In_ DWORD dwMaximumSizeHigh,
            _In_ DWORD dwMaximumSizeLow,
            _In_opt_ LPCSTR lpName
            ) -> HANDLE
        {
            return ::CreateFileMappingA(hFile, lpFileMappingAttributes, flProtect, dwMaximumSizeHigh, dwMaximumSizeLow, lpName);
        }
        
        auto FileMappingImpl::CreateFileMappingW(
            _In_ HANDLE hFile,
            _In_opt_ LPSECURITY_ATTRIBUTES lpFileMappingAttributes,
            _In_ DWORD flProtect,
            _In_ DWORD dwMaximumSizeHigh,
            _In_ DWORD dwMaximumSizeLow,
            _In_opt_ LPCWSTR lpName
            ) -> HANDLE
        {
            return ::CreateFileMappingW(hFile, lpFileMappingAttributes, flProtect, dwMaximumSizeHigh, dwMaximumSizeLow, lpName);
        }
        
        auto FileMappingImpl::MapViewOfFile(
            _In_ HANDLE hFileMappingObject,
            _In_ DWORD dwDesiredAccess,
            _In_ DWORD dwFileOffsetHigh,
            _In_ DWORD dwFileOffsetLow,
            _In_ SIZE_T dwNumberOfBytesToMap
            ) -> LPVOID
        {
            return ::MapViewOfFile(hFileMappingObject, dwDesiredAccess, dwFileOffsetHigh, dwFileOffsetLow, dwNumberOfBytesToMap);
        }
        
        auto FileMappingImpl::UnmapViewOfFile(_In_ LPCVOID lpBaseAddress) -> BOOL
        {
            return ::UnmapViewOfFile(lpBaseAddress);
        }
    }
}
