#include "debug.h"
#include <memory>
#include "error.h"

win32cpp::tracer::tracer(wchar_t const* filename, unsigned const line) : m_filename{ filename }, m_line{ line }
{
}

auto win32cpp::tracer::operator()(wchar_t const* pFormat, ...) const -> void
{
    constexpr const auto format_string = L"%s(%u): ";
    va_list args;
    va_start(args, pFormat);
    auto cch1 = _scwprintf(format_string, m_filename, m_line);
    CHECK_NE(-1, cch1);
    auto cch2 = _vscwprintf(pFormat, args);
    CHECK_NE(-1, cch2);
    auto cch = cch1 + cch2 + 1;
    auto pString = std::make_unique<wchar_t[]>(cch);
    CHECK_NE(-1, swprintf_s(pString.get(), cch, format_string, m_filename, m_line));
    CHECK_NE(-1, _vsnwprintf_s(pString.get() + cch1, static_cast<size_t>(cch) - cch1, _TRUNCATE, pFormat, args));
    OutputDebugStringW(pString.get());
    va_end(args);
}

auto win32cpp::outputDebugStringEx(wchar_t const* pFormat, ...) -> void
{
    va_list args;
    va_start(args, pFormat);
    auto cch = _vscwprintf(pFormat, args) + 1;
    CHECK_NE(-1, cch);
    auto pString = std::make_unique<wchar_t[]>(cch);
    CHECK_NE(-1, _vsnwprintf_s(pString.get(), cch, _TRUNCATE, pFormat, args));
    OutputDebugStringW(pString.get());
    va_end(args);
}
