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

#ifndef CLASS_NODE_HPP
#define CLASS_NODE_HPP

#include "VersionClassContainer.hpp"

class ClassNode :
	public std::enable_shared_from_this<ClassNode> {
public:
	
	ClassNode(const std::string& sourceFile,
			const std::string& className,
			std::shared_ptr<Compiler> compiler) :
			classVersions(sourceFile, className, compiler) {
	}
	
	inline std::shared_ptr<ClassNode> Self() {
		return std::enable_shared_from_this<ClassNode>::
			shared_from_this();
	}
	
	inline void AddChildClass(std::shared_ptr<ClassNode> ptr) {
		if(!IsClassInTree(ptr)) {
			children.insert(ptr);
			ptr->parent = Self();
		}
	}
	
	inline std::shared_ptr<ClassNode> GetParent() {
		return parent;
	}
	
	inline std::set<std::shared_ptr<ClassNode>> GetChildren() {
		return children;
	}
	
	inline size_t Update(size_t count) {
		return classVersions.Update(count);
	}
	
	inline Pointer<Object> New(ConstructorArgs... args) {
		return classVersions.New(args...);
	}
	
	bool Recompiler() {
		return classVersions.Recompiler();
	}
	
private:
	
	bool IsClassInTree(std::shared_ptr<ClassNode> ptr) {
		return IsClassInTree(ptr, NULL);
	}
	
	bool IsClassInTree(std::shared_ptr<ClassNode> ptr,
			std::shared_ptr<ClassNode> org) {
		bool result = false;
		if(parent != org)
			result |= parent->IsClassInTree(ptr, Self());
		for(auto it : children)
			if(it != ptr)
				result |= it->IsClassInTree(ptr, Self());
		return result;
	}
	
	VersionClassContainer classVersions;
	
	std::set<std::shared_ptr<ClassNode>> children;
	std::shared_ptr<ClassNode> parent;
};

#endif

