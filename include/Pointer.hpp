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

#ifndef POINTER_HPP
#define POINTER_HPP

#include <atomic>
#include <cinttypes>
#include <cstddef>
#include <cmath>

template<typename T>
class Pointer {
public:
	
	class Object {
	public:
		Object(T* _ptr=NULL) {
			ptr = _ptr;
			references = 0;
		}
		
		inline void Decrement() {
			--references;
			if(references == 0)
				delete this;
		}
		
		inline void AddOne() {
			++references;
		}
		
		inline T* ReplaceWith(T* newPtr) {
			T* ret = ptr;
			ptr = newPtr;
		}
		
		inline void Increment() {
			++references;
		}
		
		friend class Pointer<T>;
		
	private:
		~Object() {
			if(ptr)
				delete ptr;
			ptr = NULL;
			references = 0;
		}
		T* ptr;
		std::atomic<size_t> references;
	};
	
	Pointer(T* ptr) {
		if(ptr) {
			self = new Object(ptr);
			self->Increment();
		} else {
			self = NULL;
		}
	}
	
	Pointer() : self(NULL) {
	}
	
	Pointer(Pointer<T>& other) {
		self = other.self;
		if(self)
			self->Increment();
	}
	
	Pointer(const Pointer<T>& other) {
		self = (Object*)other.self;
		if(self)
			self->Increment();
	}
	
	Pointer(Pointer<T>&& other) {
		self = other.self;
	}
	
	~Pointer() {
		RemoveRef();
	}
	
	inline void RemoveRef() {
		if(self)
			self->Decrement();
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
	
private:
	
	struct Hash {
		size_t operator()(const Pointer<T>& value) const {
			constexpr const static int shift = log2(1+sizeof(T));
			return ((size_t)value.self) >> shift;
		}
	};
	
	Object* self;
};

#endif

