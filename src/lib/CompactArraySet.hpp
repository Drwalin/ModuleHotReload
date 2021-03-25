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

#ifndef COMPACT_ARRAY_MAP_HPP
#define COMPACT_ARRAY_MAP_HPP

#include <algorithm>

template<typename T>
class CompactArray {
public:
	
	CompactArray() {
		_data=NULL;
		_size=0;
	}
	CompactArray(const CompactArray&) = delete;
	CompactArray(CompactArray&) = delete;
	CompactArray(CompactArray&&) = delete;
	~CompactArray() {
		clear();
	}
	
	inline CompactArray& operator=(CompactArray&& other) {}
	inline CompactArray& operator=(CompactArray& other) {}
	inline CompactArray& operator=(const CompactArray& other) = delete;
	
	
	inline void clear() {
		if(_data) {
			delete[] _data;
			_size = 0;
			_data = NULL;
		}
	}
	
	inline void insert(T* pos, const T* first, const T* last) {
		if(first==last || !first || !last)
			return;
		size_t new_size = _size + last-first;
		T* src1 = begin();
		T* src2 = (T*)first;
		T* mid = pos;
		T* new_data = new T[new_size];
		T* dst = new_data;
		for(; src1!=mid; ++src1, ++dst)
			*dst = *src1;
		for(; src2!=last; ++src2, ++dst)
			*dst = *src2;
		for(; src1!=end(); ++src1, ++dst)
			*dst = *src1;
		delete[] _data;
		_data = new_data;
		_size = new_size;
	}
	
	inline void erase(T* it) {
		erase(it, 1);
	}
	
	inline void erase(T* first, T* last) {
		erase(first, last-first);
	}
	
	inline void erase(T* it, size_t elements) {
		if(_data && it && it!=end()) {
			if(elements > end()-it)
				elements = end()-it;
			T* new_end = end()-elements;
			size_t new_size = _size-elements;
			if(new_size == 0) {
				clear();
				return;
			}
			T* data = new T[new_size];
			T* dst = data;
			T* src = _data;
			for(; src!=it; ++src, ++dst)
				*dst = *src;
			for(src=it+elements; src!=end(); ++src, ++dst)
				*dst = *src;
			delete[] _data;
			_data = data;
			_size = new_size;
		}
	}
	
	inline void resize(size_t new_size) {
		if(new_size == 0) {
			clear();
		} else {
			T* new_data = new T[new_size];
			for(size_t i=0; i<_size && i<new_size; ++i)
				new_data[i] = _data[i];
			delete[] _data;
			_data = new_data;
			_size = new_size;
		}
	}
	
	inline T* data() { return _data; }
	inline size_t size() const { return _size; }
	inline const T* begin() const { return _data; }
	inline const T* end() const { return _data+_size; }
	inline T* begin() { return _data; }
	inline T* end() { return _data+_size; }
	
protected:
	
	T* _data;
	size_t _size;
};

template<typename T, typename Comp, Comp Comparator>
class CompactSet : public CompactArray<T> {
public:
	
	template<typename sT>
	class CompactSetSubRange {
	public:
		
		~CompactSetSubRange() {};
		
		inline T* begin() { return lower; }
		inline T* end() { return upper; }
		inline T*const begin() const { return lower; }
		inline T*const end() const { return upper; }
		
		inline T* find_equal(const T& value) {
			for(T* it : *this)
				if(*it == value)
					return *it;
			return NULL;
		}
		
		friend class CompactSet<T, Comp, Comparator>;
	private:
		
		CompactSetSubRange(sT*l, sT*r) : lower(l), upper(r) {}
		
		sT* lower;
		sT* upper;
	};
	
	inline void erase(const T&value) {
		auto all = all_of(value);
		std::cout << all.end()-all.begin();
		for(int e : all)
			std::cout<<"   " <<e;
		CompactArray<T>::erase(all.begin(), all.end());
	}
	
	inline void insert(const T* first, const T* last) {
		CompactArray<T>::insert(end(), first, last);
		sort();
	}
	
	inline void insert(const T& value) {
		insert(&value, (&value)+1);
	}
	
	inline void insert(T& value) {
		insert(&value, (&value)+1);
	}
	
	
	inline void sort() {
		std::sort(begin(), end(), Comparator);
	}
	
	inline T* lower_bound(const T& value) { return std::lower_bound(begin(), end(), value, Comparator); }
	inline T* upper_bound(const T& value) { return std::upper_bound(begin(), end(), value, Comparator); }
	inline T* find(const T& value) {
		T* ret = std::lower_bound(begin(), end(), Comparator);
		if(ret == end())
			return NULL;
		return ret;
	}
	inline T* find_any(const T& value) { return find(value); }
	inline CompactSetSubRange<T> all_of(const T& value) {
		return CompactSetSubRange(lower_bound(value), upper_bound(value));
	}
	
	inline T*const lower_bound(const T& value) const { return std::lower_bound(begin(), end(), value, Comparator); }
	inline T*const upper_bound(const T& value) const { return std::upper_bound(begin(), end(), value, Comparator); }
	inline T*const find(const T& value) const {
		T* ret = std::lower_bound(begin(), end(), Comparator);
		if(ret == end())
			return NULL;
		return ret;
	}
	inline T*const find_any(const T& value) const { return find(value); }
	inline CompactSetSubRange<const T> all_of(const T& value) const {
		return CompactSetSubRange(lower_bound(value), upper_bound(value));
	}
	
	inline T* data() { return this->_data; }
	inline size_t size() const { return this->_size; }
	inline const T* begin() const { return this->_data; }
	inline const T* end() const { return this->_data+this->_size; }
	inline T* begin() { return this->_data; }
	inline T* end() { return this->_data+this->_size; }
};

#endif

