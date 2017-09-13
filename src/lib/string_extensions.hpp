#pragma once
#include <algorithm>


namespace vxshared
{
	template<typename... Args>
	std::wstring appendPath(Args const&... args)
	{
		std::wstring result;
		int unpack[]{ 0, (result += result.empty() ? trimPath(to_wstring(args)) : L'\\' + trimPath(to_wstring(args)), 0)... };
		return result;
	}

	_When_(_Param_(1) == 0, _Post_equal_to_(0))
	_Check_return_ _CRT_JIT_INTRINSIC _ACRTIMP inline int __cdecl ispath(_In_ int _C)
	{
		return _C == L'\\' || _C == L'/';
	}

	auto str2wstr(const std::string&) -> std::wstring;

	inline std::string const& to_string(std::string const& s) { return s; }

	inline std::wstring const& to_wstring(std::wstring const& s) { return s; }

	template <typename T>
	auto trim(const std::basic_string<T> & s) -> std::basic_string<T>
	{
		auto front = std::find_if_not(begin(s), end(s), isspace);
		auto back = std::find_if_not(rbegin(s), rend(s), isspace);
		return std::basic_string<T>{ front, back.base() };
	}

	template <typename T>
	auto trimLeft(const std::basic_string<T> & s) -> std::basic_string<T>
	{
		auto front = std::find_if_not(begin(s), end(s), isspace);
		return std::basic_string<T>{ front, end(s) };
	}

	template <typename T>
	auto trimPath(const std::basic_string<T> & s) -> std::basic_string<T>
	{
		auto front = std::find_if_not(begin(s), end(s), ispath);
		auto back = std::find_if_not(rbegin(s), rend(s), ispath);
		return std::basic_string<T>{ front, back.base() };
	}

	template <typename T>
	auto trimRight(const std::basic_string<T> & s) -> std::basic_string<T>
	{
		auto back = std::find_if_not(rbegin(s), rend(s), isspace);
		return std::basic_string<T>{ begin(s), back.base() };
	}

	auto wstr2str(const std::wstring&) -> std::string;
}
