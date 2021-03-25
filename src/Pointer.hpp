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

#ifndef VTABLE_POINTER_HPP
#define VTABLE_POINTER_HPP

#include "Object.hpp"
#include "TypeAsserts.hpp"

struct Pointer {
public:
	
	Pointer() : self(NULL) {
	}
	Pointer(Object* Object) {
		self = Object;
		if(self)
			self->references++;
	}
	Pointer(Pointer& other) {
		self = other.self;
		if(self)
			self->references++;
	}
	Pointer(const Pointer& other) {
		self = (Object*)other.self;
		if(self)
			self->references++;
	}
	Pointer(Pointer&& other) {
		self = other.self;
	}
	~Pointer() {
		if(self)
			self->RemoveReference();
	}
	
	inline Pointer& operator=(Pointer& other) {
		self = other.self;
		if(self)
			self->references++;
		return *this;
	}
	inline Pointer& operator=(const Pointer& other) {
		self = (Object*)other.self;
		if(self)
			self->references++;
		return *this;
	}
	inline Pointer& operator=(Pointer&& other) {
		self = other.self;
		return *this;
	}
		
	
	inline VTable* GetVTable() const {
		if(self)
			return *(VTable**)self;
	}
	
	template<typename T, typename... Args>
	inline T Call(size_t methodId, Args... args) {
		static_assert(InvalidArgumentTypes<Args...>::sum==0, "Argument can be only of pointer or primitive type");
		return (
				(T __FASTCALL (*)(void*,Args...))
				(self->vtable()->
				 methods[methodId]
				)
			   )(self->object(), args...);
	}
	
	inline operator bool() const {
		return self && self->self;
	}
	
	Object* self;
};

#endif

