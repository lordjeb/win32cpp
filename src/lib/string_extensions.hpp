#pragma once
#include <algorithm>
#include <string>

namespace win32cpp
{
	// Appends two sections of path together with a backslash separator, ensuring that
	// there are no extra separators
	template<typename... Args>
	std::wstring appendPath(Args const&... args)
	{
		std::wstring result;
		int unpack[]{ 0,
			(result += result.empty()
				? trimPath(to_wstring(args))
				: L'\\' + trimPath(to_wstring(args)), 0
			)... };
		return result;
	}

	_When_(_Param_(1) == 0, _Post_equal_to_(0))
	_Check_return_ _CRT_JIT_INTRINSIC inline int __cdecl ispath(_In_ int _C)
	{
		return _C == L'\\' || _C == L'/';
	}

	// Convert a std::string to a std::wstring
	auto str2wstr(const std::string&) -> std::wstring;

	inline std::wstring const& to_wstring(std::wstring const& s) { return s; }

	// Trim whitespace characters from the beginning and end of a string
	template <typename T>
	auto trim(const std::basic_string<T> & s) -> std::basic_string<T>
	{
		auto front = std::find_if_not(begin(s), end(s), isspace);
		auto back = std::find_if_not(rbegin(s), rend(s), isspace);
		return std::basic_string<T>{ front, back.base() };
	}

	// Trim whitespace characters from the beginning of a string
	template <typename T>
	auto trimLeft(const std::basic_string<T> & s) -> std::basic_string<T>
	{
		auto front = std::find_if_not(begin(s), end(s), isspace);
		return std::basic_string<T>{ front, end(s) };
	}

	// Trim path separator characters from the beginning and end of a string
	template <typename T>
	auto trimPath(const std::basic_string<T> & s) -> std::basic_string<T>
	{
		auto front = std::find_if_not(begin(s), end(s), ispath);
		auto back = std::find_if_not(rbegin(s), rend(s), ispath);
		return std::basic_string<T>{ front, back.base() };
	}

	// Trim whitespace from the end of a string
	template <typename T>
	auto trimRight(const std::basic_string<T> & s) -> std::basic_string<T>
	{
		auto back = std::find_if_not(rbegin(s), rend(s), isspace);
		return std::basic_string<T>{ begin(s), back.base() };
	}

	// Convert a std::wstring to a std::string
	auto wstr2str(const std::wstring&) -> std::string;
}
