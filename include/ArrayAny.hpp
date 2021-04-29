/*
 *  This file is part of ModuleHotReload. Please see README for details.
 *  Copyright (C) 2021 Marek Zalewski aka Drwalin
 *
 *  This is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef ARRAY_ANY_HPP
#define ARRAY_ANY_HPP

#include <vector>
#include <typeinfo>
#include <cinttypes>
#include <cstddef>
#include <typeindex>
#include <type_traits>

#include <iostream>

class ArrayAny {
	
	class Any {
	public:
		Any() : type(typeid(void)) {ref = NULL;}
		Any(const Any&) = default;
		Any(Any&) = default;
		Any(Any&&) = default;
		Any& operator=(Any&) = default;
		Any& operator=(const Any&) = default;
		Any& operator=(Any&&) = default;
		
		template<typename T>
		Any(T& arg) : type(typeid(T)) {ref = &arg;}
		
		template<typename T>
		inline void Set(T& arg) {type = typeid(T);ref = &arg;}
		
		template<typename T>
		inline bool IsOfType() const {return type==typeid(T) && ref!=NULL;}
		
		template<typename T>
		inline T* Cast() {return static_cast<T*>(ref);}
		
	private:
		std::type_index type;
		void* ref;
	};
	
public:
	
	template<typename... Args>
	ArrayAny(Args&... args) : array({args...}) {}
	ArrayAny(const ArrayAny&) = default;
	ArrayAny(ArrayAny&&) = default;
	
	ArrayAny& operator=(ArrayAny&) = default;
	ArrayAny& operator=(const ArrayAny&) = default;
	ArrayAny& operator=(ArrayAny&&) = default;
	
	void Append(ArrayAny& other) {
		array.insert(array.end(), other.array.begin(), other.array.end());
	}
	
	template<typename T>
	T* Get(size_t id) {
		for(Any& it : array) {
			if(it.IsOfType<T>()) {
				if(id == 0)
					return it.Cast<T>();
				--id;
			}
		}
		return NULL;
	}
	
	template<typename T>
	T&& Move(size_t id) {
		T* ptr = Get<T>(id);
		if(ptr)
			return std::move(*ptr);
		return T();
	}
	
	template<typename T>
	bool MoveTo(size_t id, T& dst) {
		T* ptr = Get<T>(id);
		if(ptr) {
			dst = std::move(*ptr);
			return true;
		}
		dst = T();
		return false;
	}
	
	template<typename... Args>
	bool MoveToAll(Args&... args) {
		return MoveToThis(0, args...);
	}
	
private:
	
	template<typename T, typename... Args>
	bool MoveToThis(size_t pos, T& first, Args&... args) {
		if(array.size() > pos) {
			T* ptr = array[pos].Cast<T>();
			if(ptr) {
				first = std::move(*ptr);
				return MoveToThis(pos+1, args...);
			}
			MoveToThis(pos, first, args...);
			return false;
		}
		return false;
	}
	
	bool MoveToThis(size_t) {
		return true;
	}
	
	std::vector<Any> array;
};

#endif

