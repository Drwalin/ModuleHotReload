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

#include <iostream>

template <class T >
inline std::string MethodName(const std::string& prettyFunction) {
	std::string ret = prettyFunction;
	size_t index = 0;
	
	while(true) {
		index = ret.find("::__cxx11");
		if(index == std::string::npos)
			break;
		ret.replace(index, 9, "");
	}
	
	index = ret.find(" [");
	if(index != std::string::npos)
		ret.resize(index);
	
	return ret;
}
#define __METHOD_NAME__ (MethodName<int>(__PRETTY_FUNCTION__))

#define DEBUG(x) { std::cerr << " " << __METHOD_NAME__ << "(" << (x) << ") " << __FILE__ << ":" << __LINE__ << "\n"; std::cerr.flush(); }
#define MESSAGE(x) { std::cerr << " " << __METHOD_NAME__ << "(" << (x) << ") " << __FILE__ << ":" << __LINE__ << "\n"; std::cerr.flush();  }

template<typename T>
class Pointer {
public:
	
	class Object {
	public:
		Object(T* _ptr=NULL) {
//			MESSAGE(references);
			ptr = _ptr;
			references = 0;
		}
		
		inline void Decrement() {
//			MESSAGE(references);
			--references;
			if(references == 0)
				delete this;
		}
		
		inline void AddOne() {
//			MESSAGE(references);
			++references;
		}
		
		inline T* ReplaceWith(T* newPtr) {
//			MESSAGE(references);
			T* ret = ptr;
			ptr = newPtr;
			return ret;
		}
		
		inline void Increment() {
//			MESSAGE(references);
			++references;
		}
		
		friend class Pointer<T>;
		
	private:
		~Object() {
//			MESSAGE(references);
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
//			MESSAGE(self?(long long)(self->references):-1);
	}
	
	Pointer() : self(NULL) {
//			MESSAGE(self?(long long)(self->references):-1);
	}
	
	Pointer(Pointer<T>& other) {
		self = other.self;
		if(self)
			self->Increment();
//			MESSAGE(self?(long long)(self->references):-1);
	}
	
	Pointer(const Pointer<T>& other) {
		self = (Object*)other.self;
		if(self)
			self->Increment();
//			MESSAGE(self?(long long)(self->references):-1);
	}
	
	Pointer(Pointer<T>&& other) {
		self = other.self;
//			MESSAGE(self?(long long)(self->references):-1);
	}
	
	~Pointer() {
//			MESSAGE(self?(long long)(self->references):-1);
		RemoveRef();
//			MESSAGE(self?(long long)(self->references):-1);
	}
	
	inline void RemoveRef() {
//			MESSAGE(self?(long long)(self->references):-1);
		if(self)
			self->Decrement();
		self = NULL;
	}
	
	
	inline Pointer<T>& operator=(Pointer<T>& other) {
		RemoveRef();
		self = other.self;
		if(self)
			self->AddOne();
//			MESSAGE(self?(long long)(self->references):-1);
		return *this;
	}
	
	inline Pointer<T>& operator=(const Pointer<T>& other) {
		RemoveRef();
		self = (Object*)other.self;
		if(self)
			self->AddOne();
//			MESSAGE(self?(long long)(self->references):-1);
		return *this;
	}
	
	inline Pointer<T>& operator=(Pointer<T>&& other) {
		RemoveRef();
		self = other.self;
//			MESSAGE(self?(long long)(self->references):-1);
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
	inline T& operator*(int) {return *self->ptr;}
	inline const T& operator*(int) const {return *self->ptr;}
	
	inline operator bool() const {
		return self && self->ptr;
	}
	
	inline T* ReplaceWith(T* newPointer) {
//			MESSAGE(self?(long long)(self->references):-1);
		if(!self) {
			this->operator=(Pointer<T>(newPointer));
//			MESSAGE(self?(long long)(self->references):-1);
			return NULL;
		}
		return self->ReplaceWith(newPointer);
	}
	
	struct Hash {
		inline size_t operator()(const Pointer<T>& value) const {
			constexpr const static int shift = log2(1+sizeof(T));
			return ((size_t)value.self) >> shift;
		}
	};
	
	inline T* _get() {
//			MESSAGE(self?(long long)(self->references):-1);
		return self->ptr;
	}
	
	inline const T* _get() const {
//			MESSAGE(self?(long long)(self->references):-1);
		return self->ptr;
	}
	
private:
	
	Object* self;
};

#include <functional>

namespace std {
	template<typename T>
	struct hash<Pointer<T>> : public Pointer<T>::Hash {
		using size_type = size_t;
		using argument_type = Pointer<T>;
	};
}

#endif

