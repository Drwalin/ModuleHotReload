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

#ifndef VTABLE_OBJECT_HPP
#define VTABLE_OBJECT_HPP

#include <cinttypes>
#include <memory>
#include <string>

#include "VTable.hpp"

class Object {
public:
	
	std::atomic<size_t> references;
	void* self;
	
	Object() {
		references = 0;
		self = NULL;
	}
	Object(void* ptr) {
		references = 0;
		self = ptr;
	}
	Object(const Object&) = delete;
	Object(Object&) = delete;
	Object(Object&&) = delete;
	Object& operator=(Object&) = delete;
	Object& operator=(const Object&) = delete;
	Object& operator=(Object&&) = delete;
	
	inline void SetPtr(void* ptr) {
		Delete();
		self = ptr;
	}
	
	inline void New(VTable* vtable) {
		Delete();
		if(vtable) {
			self = vtable->allocate();
		}
	}
	
	inline void AddReference() {
		references++;
	}
	
	inline void RemoveReference() {
		if(self) {
			references--;
			if(references == 0) {
				Delete();
				delete this;
			}
		}
	}
	
	inline VTable* GetVTable() {
		if(self)
			return *(VTable**)self;
		return NULL;
	}
	
	inline void Delete() {
		if(self) {
			GetVTable()->free(self);
			self = NULL;
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

#endif

