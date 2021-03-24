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

#ifndef VTABLE_HPP
#define VTABLE_HPP

#include <cinttypes>
#include <memory>
#include <string>
#include <cstdlib>
#include <atomic>

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
			return (ptr->*member)(args...);
		}
	};
};
template<typename T, class Class, typename... Args>
inline CLASS<T, Class, Args...> GetCLASS(T (Class::*member)(Args...)) {
	return CLASS<T, Class, Args...>();
}
#define GET_FUNCTION(Class, Method) \
	decltype(GetCLASS(&Class::Method))::INTERN<&Class::Method>::Call

struct VTable {
	void* (*allocate)();
	void (*free)(void*);
	size_t size;
	std::string name;
	void** methods;
	std::atomic<size_t> numberOfObjects;
};



class Object {
public:
	std::atomic<size_t> references;
	void* self;
	
	Object() {
		references = 0;
		self = NULL;
	}
	
	inline void AddReference() {
		references++;
	}
	
	inline void RemoveReference() {
		if(self) {
			references--;
			if(references == 0) {
				((VTable*)self)->free(self);
				free(this);
			}
		}
	}
	
private:
	~Object() {
		if(self) {
			((VTable*)self)->free(self);
			self = NULL;
		}
	}
};

class Pointer {
public:
	
	Pointer() : self(NULL) {
	}
	Pointer(Object* Object) {
		self=Object;
		if(self)
			self->references++;
	}
	Pointer(Pointer& other) {
		self=other.self;
		if(self)
			self->references++;
	}
	Pointer(const Pointer& other) {
		self=(Object*)other.self;
		if(self)
			self->references++;
	}
	Pointer(Pointer&& other) {
		self=other.self;
	}
	~Pointer() {
		if(self) {
			self->RemoveReference();
		}
	}
	
	Pointer& operator=(Pointer& other) {
		self=(Object*)other.self;
		if(self)
			self->references++;
		return *this;
	}
	Pointer& operator=(const Pointer& other) {
		self=(Object*)other.self;
		if(self)
			self->references++;
		return *this;
	}
	Pointer& operator=(Pointer&& other) {
		self=(Object*)other.self;
		return *this;
	}
		
	
	VTable* GetVTable() const {
		if(self) {
			return *(VTable**)self;
		}
	}
	
	template<typename T, typename... Args>
	T Call(size_t methodId, Args... args) {
		return (
				(T __FASTCALL (*)(void*,Args...))
				((*((VTable**)(self->self)))->
				 methods[methodId]
				)
			   )((void*)((size_t)self->self+sizeof(void*)), args...);
	}
	
	inline operator bool() const {
		return self && self->self;
	}
	
	Object* self;
};

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
		Class##_AllocateObject, \
		Class##_FreeObject, \
		sizeof(Class), \
		#Class, \
		Class##_methods \
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

