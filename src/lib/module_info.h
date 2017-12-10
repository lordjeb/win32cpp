#pragma once
#include "handle.h"

namespace win32cpp
{
    auto getModuleFilename() -> std::wstring;
    auto getModulePath() -> std::wstring;
    auto getTempFilename(const std::wstring& path, const std::wstring& prefix, unsigned int unique = 0) -> std::wstring;
    auto getTempPath() -> std::wstring;
    auto getWindowsPath() -> std::wstring;
    auto getSystemPath() -> std::wstring;
    auto getThreadToken(DWORD DesiredAccess = TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY) -> unique_token_handle;
}
