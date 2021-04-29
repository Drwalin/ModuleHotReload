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

#ifndef CLASSES_TREE_HPP
#define CLASSES_TREE_HPP

#include "ClassNode.hpp"
#include "CompilerThread.hpp"

#include <string>
#include <map>
#include <limits>

class ClassesTree {
public:
	
	ClassesTree(std::shared_ptr<Compiler> compiler) : compiler(compiler) {
	}
	
	~ClassesTree() {
	}
	
	
	std::shared_ptr<ClassNode> GetClass(const std::string& className) {
		auto it = classes.find(className);
		if(it == classes.end())
			return NULL;
		return it->second;
	}
	
	inline size_t UpdateObjects(size_t count) {
		if(count == 0)
			count = std::numeric_limits<size_t>::max();
		auto it = classes.find(lastUpdatedClass);
		if(it == classes.end())
			it = classes.begin();
		for(; it!=classes.end() && count!=0; ++it)
			count = it->second->Update(count);
		if(it != classs.end())
			lastUpdatedClass = it->first;
		else
			lastUpdatedClass = "";
		return count;
	}
	
	inline bool AddClass(const std::string& parentClassName,
			const std::string& className,
			const std::string& sourceFile) {
		auto it = classes.find(className);
		if(it != classes.end())
			return false;
		
		auto newClass = new ClassNode(sourcFile, className, compiler);
		classes[className] = newClass;
		auto parent_it = classes.find(parentClassName);
		if(parent_it != classes.end())
			parent_it->AddChildClass(newClass);
		return true;
	}
	
	inline void RequestRecompile(const std::string& className) {
		auto it = classes.find(classname);
		if(it != classes.end())
			compilerThread.RequestRecompile(it->second);
	}
	
private:
	
	std::map<std::string, std::shared_ptr<ClassNode>> classes;
	std::shared_ptr<Compiler> compiler;
	CompilerThread compilerThread;
	std::string lastUpdatedClass;
};

#endif

