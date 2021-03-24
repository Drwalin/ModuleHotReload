/*
 *  This file is part of ModulHotReload. Please see README for details.
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

#ifndef TYPE_ASSERTS_HPP
#define TYPE_ASSERTS_HPP

#include <type_traits>

#include "TypeName.hpp"

template<typename T>
struct  PointerOrPrimitiveArgument {
	inline const static int sum = !((
		std::is_pointer<T>::value ||
		std::is_fundamental<T>::value) &&
		!std::is_reference<T>::value);
};

template<typename... C>
struct AssertPointerOrPrimitiveArgumentTypes_internal;


template<typename C, typename T, typename... Args>
struct AssertPointerOrPrimitiveArgumentTypes_internal<C, T, Args...> {
		inline const static  int sum =
		PointerOrPrimitiveArgument<T>::sum +
		AssertPointerOrPrimitiveArgumentTypes_internal<C, Args...>::sum;
};

template<typename C>
struct AssertPointerOrPrimitiveArgumentTypes_internal<C> {
	inline const static int sum = 0;
};

template<typename... Args>
struct InvalidArgumentTypes {
	inline const static int sum =
	AssertPointerOrPrimitiveArgumentTypes_internal<void, Args...>::sum;
};

#endif

