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

#include "Class.hpp"

template<typename Base, typename... ConstructorArgs>
Class<Base, ConstructorArgs...>::Class(
		std::shared_ptr<Dll> dll,
		const std::string& className,
		size_t typeSize,
		std::shared_ptr<ConstructorBase<Base, ConstructorArgs...>> allocator) :
		std::enable_shared_from_this<Class<Base, ConstructorArgs...>>(),
		dll(dll),
		_name(className),
		_size(typeSize),
		allocator(allocator) {
}

template<typename Base, typename... ConstructorArgs>
void Class<Base, ConstructorArgs...>::Destructor(Class<Base, ConstructorArgs...>* ptr) {
	delete ptr;
}

template<typename Base, typename... ConstructorArgs>
size_t Class<Base, ConstructorArgs...>::UpdateObjectsAndClearClasses(size_t max,
		UpdatePolicy updatePolicy) {
	if(max == 0)
		max = -1;
	if(next)
		return next->UpdateObjectsAndClearClasses(max, updatePolicy);
	return max - InternalUpdateAndClearClasses(max, updatePolicy, Self());
}

template<typename Base, typename... ConstructorArgs>
std::shared_ptr<Class<Base, ConstructorArgs...>> Class<Base, ConstructorArgs...>::AddNewVersion(
		std::shared_ptr<Class<Base, ConstructorArgs...>> newClass) {
	if(next)
		return next->AddNewVersion(newClass);
	next = newClass;
	if(newClass == NULL)
		return Self();
	newClass->prev = Self();
	return newClass;
}

template<typename Base, typename... ConstructorArgs>
std::shared_ptr<Class<Base, ConstructorArgs...>> Class<Base, ConstructorArgs...>::Self() {
	return std::enable_shared_from_this<Class<Base, ConstructorArgs...>>::shared_from_this();
}

template<typename Base, typename... ConstructorArgs>
size_t Class<Base, ConstructorArgs...>::size() const {
	return _size;
}

template<typename Base, typename... ConstructorArgs>
std::shared_ptr<Dll> Class<Base, ConstructorArgs...>::GetDll() {
	return dll;
}

template<typename Base, typename... ConstructorArgs>
Class<Base, ConstructorArgs...>::~Class() {
}


template<typename Base, typename... ConstructorArgs>
void Class<Base, ConstructorArgs...>::AddParentClass(std::shared_ptr<ClassType> parentClass) {
	if(!IsClassInATree(parentClass)) {
		directParentClasses.insert(parentClass);
		parentClass->AddChildClass(Self());
	}
}

template<typename Base, typename... ConstructorArgs>
void Class<Base, ConstructorArgs...>::AddChildClass(std::shared_ptr<ClassType> childClass) {
	if(!IsClassInATree(childClass)) {
		directChildrenClasses.insert(childClass);
		childClass->AddParentClass(Self());
	}

}

template<typename Base, typename... ConstructorArgs>
void Class<Base, ConstructorArgs...>::GetChildrenClasses(std::set<std::shared_ptr<ClassType>>& classes) {
	classes.insert(Self());
	for(auto it : directChildrenClasses)
		it->GetChildrenClasses(classes);
}


template<typename Base, typename... ConstructorArgs>
bool Class<Base, ConstructorArgs...>::IsClassInATree(std::shared_ptr<ClassType> newClass) {
	return IsClassIndirectParent(newClass) | IsClassIndirectChild(newClass);
}

template<typename Base, typename... ConstructorArgs>
bool Class<Base, ConstructorArgs...>::IsClassIndirectParent(std::shared_ptr<ClassType> parentClass) {
	if(Self() == parentClass)
		return true;
	bool result = false;
	for(auto it : directParentClasses)
		result |= it->IsClassIndirectParent(parentClass);
	return result;
}

template<typename Base, typename... ConstructorArgs>
bool Class<Base, ConstructorArgs...>::IsClassIndirectChild(std::shared_ptr<ClassType> childClass) {
	if(Self() == childClass)
		return true;
	bool result = false;
	for(auto it : directChildrenClasses)
		result |= it->IsClassIndirectParent(childClass);
	return result;
}



template<typename Base, typename... ConstructorArgs>
void Class<Base, ConstructorArgs...>::RemoveParentClass(std::shared_ptr<ClassType> parentClass) {
	auto it = directParentClasses.find(parentClass);
	if(it != directParentClasses.end()) {
		directParentClasses.erase(it);
		parentClass->RemoveChildClass(Self());
	} else {
		for(auto p : directParentClasses)
			p->RemoveParentClass(parentClass);
	}
}

template<typename Base, typename... ConstructorArgs>
void Class<Base, ConstructorArgs...>::RemoveChildClass(std::shared_ptr<ClassType> childClass) {
	auto it = directChildrenClasses.find(childClass);
	if(it != directChildrenClasses.end()) {
		directChildrenClasses.erase(it);
		childClass->RemoveParentClass(Self());
	} else {
		for(auto p : directChildrenClasses)
			p->RemoveChildClass(childClass);
	}
}



template<typename Base, typename... ConstructorArgs>
size_t Class<Base, ConstructorArgs...>::InternalUpdateAndClearClasses(size_t max,
		UpdatePolicy updatePolicy,
		std::shared_ptr<Class<Base, ConstructorArgs...>> newestClass) {
	if(prev) {
		max = prev->InternalUpdateAndClearClasses(max, updatePolicy, newestClass);
		if(prev->_objects.empty() && prev->prev==NULL) {
			std::shared_ptr<Dll> pdll = prev->dll;
			prev->next = NULL;
			prev = NULL;
		}
	}
	if(next == NULL)
		return max;
	auto itbeg = _objects.begin();
	auto it = itbeg;
	for(; max!=0 && it != _objects.end(); --max, ++it) {
		if(it == _objects.end())
			break;
		Pointer<Base> ptr = *it;
		Base* oldPtr = NULL;
		if(updatePolicy == UpdateToImmediateNext) {
			oldPtr = ptr.ReplaceWith(next->allocator->Reallocate(ptr));
			next->_objects.insert(ptr);
		} else {
			oldPtr = ptr.ReplaceWith(newestClass->allocator->Reallocate(ptr));
			newestClass->_objects.insert(ptr);
		}
		if(oldPtr) {
			delete oldPtr;
		}
	}
	if(itbeg!=it && itbeg!=_objects.end())
		_objects.erase(itbeg, it);
	return max;
}

#define INSTANTIATE_METHODS(TYPE) \
		template \
		extern TYPE::Class( \
				std::shared_ptr<Dll> dll, \
				const std::string& className, \
				size_t typeSize, \
				std::shared_ptr<TYPE::ConstructorType> allocator); \
		\
		template \
		extern void TYPE::Destructor(TYPE* ptr); \
		\
		template \
		extern size_t TYPE::UpdateObjectsAndClearClasses( \
				size_t max, UpdatePolicy updatePolicy); \
		\
		template \
		extern std::shared_ptr<TYPE> TYPE::AddNewVersion( \
				std::shared_ptr<TYPE> newClass); \
		\
		template \
		extern std::shared_ptr<TYPE> TYPE::Self(); \
		\
		template \
		extern size_t TYPE::size() const; \
		\
		template \
		extern std::shared_ptr<Dll> TYPE::GetDll(); \
		\
		template \
		extern TYPE::~Class(); \
		\
		template \
		extern size_t TYPE::InternalUpdateAndClearClasses( \
				size_t max, UpdatePolicy updatePolicy, \
				std::shared_ptr<TYPE> newestClass);

