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
#include <memory>

#include "VTable.hpp"
#include "TypeAsserts.hpp"
#include "lib/CompileTime.h"

#if defined(__GNUC__)
    #define __FASTCALL __attribute__((fastcall)) // __fastcall
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
		static T Call(Class* ptr, Args... args) {
			static_assert(InvalidArgumentTypes<Args...>::sum==0, "Argument can be only of pointer or primitive type");
			return (ptr->*member)(args...);
		}
	};
};

template<typename T, class Class, typename... Args>
inline CLASS<T, Class, Args...> GetCLASS(T (Class::*member)(Args...)) {
	return CLASS<T, Class, Args...>();
}

template<typename... Args>
constexpr size_t Max(const Args...args);
template<typename Size, typename T, typename... Args>
constexpr size_t Max(const Size id, const T a, const Args... args) {
	size_t i = Max<Args...>(args...);
	return i>id ? i : id;
}
template<>
constexpr size_t Max() {
	return 0;
}

void CreateOneMethod(void**methods, void***placeholder) {
}
template<typename Size, typename Void, typename... Args>
void CreateOneMethod(void** methods, Size id, Void ptr, Args... args) {
	methods[id] = ptr;
	CreateOneMethod(methods, args...);
}

template<typename... Args>
void** CreateMethodsPointers(Args... args) {
	const static size_t count = Max<const Args...>(args...)+1;
	static std::shared_ptr<void*[]> methods(new void*[count]);
	for(size_t i=0; i<count; ++i)
		methods[i] = NULL;
	CreateOneMethod<Args...>(methods.get(), args..., (void***)NULL);
	return methods.get();
}

extern "C" uint64_t PopNewEmptyModuleId();

#define GET_FUNCTION(Class, Method) \
	decltype(GetCLASS(&Class::Method))::INTERN<&Class::Method>::Call

#define CONCATENATE(A, B) INNER_CONCATENATE(A, B)
#define INNER_CONCATENATE(S, B) S##B

#define _(Method) ((void*)GET_FUNCTION(ClassDecl, Method))

#define GENERATE_CALLABLE_VTABLE_HEADER(Class, ...) \
	struct Class##_struct { \
		VTable* vtable; \
		Class object; \
	}; \
	using ClassDecl = Class; \
	extern "C" { \
		struct Class##_struct* Class##_AllocateObject(); \
		void Class##_FreeObject(struct Class##_struct* ptr); \
	}; \
	VTable Class##_vtable { \
		CreateMethodsPointers(__VA_ARGS__), \
		(void*(*)())Class##_AllocateObject, \
		(void(*)(void*))Class##_FreeObject, \
		sizeof(Class##_struct), \
		#Class, \
		PopNewEmptyModuleId(), \
		UNIX_TIMESTAMP \
	}; \
	extern "C" { \
		VTable* Class##_GetVTable() { \
			return &Class##_vtable; \
		} \
		Class##_struct* Class##_AllocateObject() { \
			Class##_struct * ptr = new Class##_struct{&Class##_vtable}; \
			Class##_vtable.objects.insert(ptr); \
			return ptr; \
		} \
		void Class##_FreeObject(Class##_struct* ptr) { \
			if(ptr) { \
				if(ptr->vtable == &Class##_vtable) { \
					Class##_vtable.objects.erase(ptr); \
					delete ptr; \
				} else if(ptr->vtable) \
					ptr->vtable->free(ptr); \
				else { \
					fprintf(stderr, "\n Error: calling " #Class "_FreeObject on object with null VTable."); \
				} \
			} \
		} \
	}; \

#endif

