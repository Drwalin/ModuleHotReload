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

template<class T>
struct Pointer {
public:
	
	struct Object {
		T* ptr;
		std::atomic<size_t> references;
		
		void RemoveOne() {
			--references;
			if(references == 0) {
				if(ptr) {
					delete ptr;
				}
				delete this;
			}
		}
		
		void AddOne() {
			++references;
		}
		
		T* ReplaceWith(T* newPtr) {
			T* ret = ptr;
			ptr = newPtr;
		}
	};
	
	
	Pointer() : self(NULL) {
	}
	
	Pointer(Pointer<T>& other) {
		self = other.self;
		if(self)
			self->references++;
	}
	
	Pointer(const Pointer<T>& other) {
		self = (Object*)other.self;
		if(self)
			self->references++;
	}
	
	Pointer(Pointer<T>&& other) {
		self = other.self;
	}
	
	template<typename T2>
	Pointer(Pointer<T2>::Object* object) {
		self = (Object*)object;
	}
	
	~Pointer() {
		RemoveRef();
	}
	
	inline void RemoveRef() {
		if(self)
			self->RemoveOne();
		self = NULL;
	}
	
	
	inline Pointer<T>& operator=(Pointer<T>& other) {
		RemoveRef();
		self = other.self;
		if(self)
			self->AddOne();
		return *this;
	}
	
	inline Pointer<T>& operator=(const Pointer<T>& other) {
		RemoveRef();
		self = (Object*)other.self;
		if(self)
			self->AddOne();
		return *this;
	}
	
	inline Pointer<T>& operator=(Pointer<T>&& other) {
		RemoveRef();
		self = other.self;
		return *this;
	}
	
	
	template<typename T2>
	inline bool operator==(const Pointer<T2>& other) const {
		return self == other.self;
	}
	
	template<typename T2>
	inline bool operator!=(const Pointer<T2>& other) const {
		return self != other.self;
	}
	
	template<typename T2>
	inline bool operator<(const Pointer<T2>& other) const {
		return self < other.self;
	}
	
	template<typename T2>
	inline bool operator<=(const Pointer<T2>& other) const {
		return self <= other.self;
	}
	
	template<typename T2>
	inline bool operator>=(const Pointer<T2>& other) const {
		return self >= other.self;
	}
	
	template<typename T2>
	inline bool operator>(const Pointer<T2>& other) const {
		return self > other.self;
	}
	
	
	
	template<typename T2>
	Pointer<T2> CastTo() {
		if(dynamic_cast<T2*>(self->ptr))
			return Pointer<T2>(self);
		return Pointer<T2>();
	}
	
	inline T* operator->() {return self->ptr;}
	inline const T* operator->() const {return self->ptr;}
	
	inline operator bool() const {
		return self && self->ptr;
	}
	
	struct Hash {
		size_t operator(const Pointer<T>& value) const {
			constexpr const static int shift = log2(1+sizeof(T));
			return ((size_t)value.self) >> shift;
		}
	};
	
	Object* self;
};

#endif

