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

#ifndef CLASS_HPP
#define CLASS_HPP

#include <string>
#include <set>
#include <memory>

#include <DllImporter.h>

#include "Pointer.hpp"

template<typename Base, typename... Args>
class ConstructorBase {
public:
	virtual Base* Allocate(Args... args) const = 0;
	virtual Base* Reallocate(Pointer<Base> old) const = 0;
};

template<typename Final, typename Base, typename... Args>
class Constructor : public ConstructorBase<Base, Args...>{
public:
	virtual Base* Allocate(Args... args) const override {
		return new Final(args...);
	}
	virtual Base* Reallocate(Pointer<Base> old) const override {
		return new Final(old);
	}
};

enum UpdatePolicy {
	UpdateToNewest=1,
	UpdateToImmediateNext=2
};

template<typename Base, typename... ConstructorArgs>
class Class :
	public std::enable_shared_from_this<Class<Base, ConstructorArgs...>> {
private:
	
	using ClassType = Class<Base, ConstructorArgs...>;
	using ConstructorType = ConstructorBase<Base, ConstructorArgs...>;
	using BaseType = Base;
	
	Class(
			std::shared_ptr<Dll> dll,
			const std::string& className,
			size_t typeSize,
			std::shared_ptr<ConstructorBase<Base, ConstructorArgs...>> allocator);
	
public:
	
	template<typename Final>
	static std::shared_ptr<ClassType> MakeClass(
			std::shared_ptr<Dll> dll,
			const std::string& className) {
		return std::shared_ptr<ClassType>(
				new ClassType(
					dll,
					className,
					sizeof(Final),
					std::shared_ptr<ConstructorBase<Base, ConstructorArgs...>>(
						new Constructor<Final, Base, ConstructorArgs...>())
					),
					ClassType::Destructor
				);
	}
	
	static void Destructor(ClassType* ptr);
	
	// passing 0 as arguments updates all objects
	// return number of updated objects
	size_t UpdateObjectsAndClearClasses(size_t max, UpdatePolicy updatePolicy);
	std::shared_ptr<ClassType> AddNewVersion(std::shared_ptr<ClassType> newClass);
	
	Pointer<Base> GetNew(ConstructorArgs... args) {
		if(next)
			return next->GetNew(args...);
		Pointer<Base> object(allocator->Allocate(args...));
		_objects.insert(object);
		return object;
	}
	
	std::shared_ptr<ClassType> Self();
	size_t size() const;
	inline const std::string& name() const {return _name;}
	std::shared_ptr<Dll> GetDll();
	
public:
	
	void AddParentClass(std::shared_ptr<ClassType> parentClass);
	void AddChildClass(std::shared_ptr<ClassType> childClass);
	
	bool IsClassInATree(std::shared_ptr<ClassType> newClass);
	bool IsClassIndirectParent(std::shared_ptr<ClassType> parentClass);
	bool IsClassIndirectChild(std::shared_ptr<ClassType> childClass);
	
	void GetChildrenClasses(std::set<std::shared_ptr<ClassType>>& classes);
	
	void RemoveChildClass(std::shared_ptr<ClassType> childClass);
	void RemoveParentClass(std::shared_ptr<ClassType> parentClass);
	
private:
	
	virtual ~Class();
	
	size_t InternalUpdateAndClearClasses(size_t max,
			UpdatePolicy updatePolicy,
			std::shared_ptr<ClassType> newestClass);
	
private:
	
	std::set<std::shared_ptr<ClassType>> directParentClasses;
	std::set<std::shared_ptr<ClassType>> directChildrenClasses;
	
	std::shared_ptr<ClassType> prev, next;
	std::shared_ptr<Dll> dll;
	std::string source;
	
	const std::string _name;
	const size_t _size;
	std::set<Pointer<Base>> _objects;
	std::shared_ptr<ConstructorBase<Base, ConstructorArgs...>> allocator;
};

#endif

