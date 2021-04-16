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

#ifndef CLASS_HPP
#define CLASS_HPP

#include <string>
#include <set>
#include <memory>
#include <DllImporer.h>

template<typename Base>
class Class {
public:
	
	template<typename Final>
	Class(const std::string& name) : _name(name), _size(sizeof(Final)) {
		
	}
	
	~Class() {
	}
	
	inline void Destroy() {
		
	}
	
	
	
	inline const std::string& name() const {return _name;}
	inline size_t size() const {return _size;}
	
	inline void SetDll(std::shared_ptr<Class> dll) {this->dll = dll;}
	inline void SetPrevious(std::shared_ptr<Class> prev) {this->prev = prev;}
	inline void SetNext(std::shared_ptr<Class> next) {this->next = next;}
	
private:
	
	Class *prev, *next;
	
	std::shared_ptr<Dll> dll;
	
	
	const std::string _name;
	const size_t _size;
	std::set<Pointer<Base>> _objects;
};

#endif

