/*
 *  This file is part of ModuleHotReload. Please see README for details.
 *  Copyright (C) 2021 Marek Zalewski aka Drwalin
 *
 *  ICon3 is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  ICon3 is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef TYPE_NAME_HPP
#define TYPE_NAME_HPP

#include <cstring>
#include <cstdio>
#include <string_view>

// Based on https://stackoverflow.com/questions/81870/is-it-possible-to-print-a-variables-type-in-standard-c
template<typename T>
constexpr auto type_name() noexcept {
#ifdef __clang__
	const char* name = __PRETTY_FUNCTION__;
	const size_t length = sizeof(__PRETTY_FUNCTION__)-1;
	const size_t prefix = sizeof("constexpr auto type_name() [T = ")-1;
	const size_t suffix = sizeof("]")-1;
#elif defined(__GNUC__)
	const char* name = __PRETTY_FUNCTION__;
	const size_t length = sizeof(__PRETTY_FUNCTION__)-1;
	const size_t prefix = sizeof("constexpr auto type_name() [with T = ")-1;
	const size_t suffix = sizeof("]")-1;
#elif defined(_MSC_VER)
	const char* name = __FUNCSIG__;
	const size_t length = sizeof(__FUNCSIG__)-1;
	const size_t prefix = sizeof("constexpr auto __cdecl type_name<")-1;
	const size_t suffix = sizeof(">(void) noexcept")-1;
#else
	static_assert(false, "Unknown compiler. Please specify your own code for type_name, prfix and suffix");
#endif
	const size_t new_length = length-suffix-prefix;
	return (char*const)({
		char* str = (char*)malloc(new_length+1);
		memcpy(str, name+prefix, new_length);
		str[new_length] = 0;
		str;
	});
}


template<typename T>
constexpr auto TypeName() noexcept {
#ifdef __clang__
	constexpr const char* name = __PRETTY_FUNCTION__;
	constexpr const size_t length = sizeof(__PRETTY_FUNCTION__)-1;
	constexpr const size_t prefix = sizeof("constexpr auto type_name() [T = ")-1;
	constexpr const size_t suffix = sizeof("]")-1;
#elif defined(__GNUC__)
	constexpr const char* name = __PRETTY_FUNCTION__;
	constexpr const size_t length = sizeof(__PRETTY_FUNCTION__)-1;
	constexpr const size_t prefix = sizeof("constexpr auto type_name() [with T = ")-1;
	constexpr const size_t suffix = sizeof("]")-1;
#elif defined(_MSC_VER)
	constexpr const char* name = __FUNCSIG__;
	constexpr const size_t length = sizeof(__FUNCSIG__)-1;
	constexpr const size_t prefix = sizeof("constexpr auto __cdecl type_name<")-1;
	constexpr const size_t suffix = sizeof(">(void) noexcept")-1;
#else
	static_assert(false, "Unknown compiler. Please specify your own code for type_name, prfix and suffix");
#endif
	constexpr const size_t new_length = length-suffix-prefix;
	return std::string(name+prefix-1, new_length+1);
}

#endif
