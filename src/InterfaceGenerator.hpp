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

#ifndef INTERFACE_GENERATOR_HPP
#define INTERFACE_GENERATOR_HPP

#include <cinttypes>
#include <cstdlib>

#include "VTable.hpp"
#include "TypeAsserts.hpp"
#include "lib/CompileTime.h"

#if defined(__GNUC__)
    #define __FASTCALL __fastcall
#elif defined(_MSC_VER)
    #define __FASTCALL __attribute__((fastcall))
#endif

template<typename T, class Class, typename... Args>
class CLASS {
public:
	
	CLASS() {}
	
	typedef T (Class::*memberType)(Args...);
	
	template<memberType member>
	class INTERN {
	public:
		static __FASTCALL T Call(Class* ptr, Args... args) {
			static_assert(InvalidArgumentTypes<Args...>::sum==0, "Argument can be only of pointer or primitive type");
			return (ptr->*member)(args...);
		}
	};
};

template<typename T, class Class, typename... Args>
inline CLASS<T, Class, Args...> GetCLASS(T (Class::*member)(Args...)) {
	return CLASS<T, Class, Args...>();
}

extern "C" uint64_t PopNewEmptyModuleId();

#define GET_FUNCTION(Class, Method) \
	decltype(GetCLASS(&Class::Method))::INTERN<&Class::Method>::Call

#define CONCATENATE(A, B) INNER_CONCATENATE(A, B)
#define INNER_CONCATENATE(S, B) S##B

#define _(Method) (void*)GET_FUNCTION(ClassDecl, Method)

#define GENERATE_CALLABLE_VTABLE_HEADER(Class, ...) \
	using ClassDecl = Class; \
	extern "C" { \
		void* Class##_AllocateObject(); \
		void Class##_FreeObject(void* ptr); \
	}; \
	void* Class##_methods[] = { \
		__VA_ARGS__, \
		NULL \
	}; \
	VTable Class##_vtable { \
		Class##_methods \
		Class##_AllocateObject, \
		Class##_FreeObject, \
		sizeof(Class), \
		#Class, \
		PopNewEmptyModuleId(), \
		UNIX_TIMESTAMP \
	}; \
	extern "C" { \
		VTable* Class##_GetVTable() { \
			return &Class##_vtable; \
		} \
		void* Class##_AllocateObject() { \
			void* mem = malloc(sizeof(void*)+sizeof(Class)); \
			if(mem == NULL)\
				return NULL; \
			Class##_vtable.numberOfObjects++;\
			*((void**)mem) = &Class##_vtable; \
			new ((void*)((size_t)mem+sizeof(void*))) Class(); \
			return mem; \
		} \
		void Class##_FreeObject(void* ptr) { \
			if(ptr) { \
				if((*(VTable**)ptr) == &Class##_vtable) { \
					Class##_vtable.numberOfObjects;\
						((Class*)((void*)((size_t)ptr+sizeof(void*))))->~Class(); \
					free(ptr); \
				} else if(*(VTable**)ptr) \
					(*(VTable**)ptr)->free(ptr); \
				else { \
					fprintf(stderr, "\n Error: calling " #Class "_FreeObject on object with null VTable."); \
				} \
			} \
		} \
	}; \

#endif

