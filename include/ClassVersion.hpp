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

#ifndef CLASS_VERSION_HPP
#define CLASS_VERSION_HPP

#include <string>
#include <vector>
#include <memory>

#include <DllImporter.h>

class Object {
public:
	
	Object() = default;
	virtual ~Object() = default;
	
	virtual class ClassVersion* _RTTI(class ClassVersion* classPtr = NULL)
		const = 0;
};

#define CLASS_RTTI public: virtual class ClassVersion* _RTTI( \
		class ClassVersion* classPtr) const override { \
			static class ClassVersion* ptr = NULL; \
			if(ptr == NULL) \
				ptr = classPtr; \
			return ptr; \
		}

#define CLASS_CONSTRUCTOR(CLASS) void ClassInitializer_##CLASS(ClassVersion* \
		classPtr) { \
			classPtr->Init<CLASS>(); \
		}

class Constructor {
public:
	
	Constructor(Object* (*_new)(), Object* (*_renew)(Pointer<Object>)) :
		_new(_new), _renew(_renew) {
	}
	
	~Constructor() = default;
	
	inline Object* New() const {
		return _new();
	}
	
	inline Object* Renew(Pointer<Object> old) const {
		return _renew(old);
	}
	
private:
	
	const Object* (*_new)();
	const Object* (*_renew)(Pointer<Object>);
};

class ClassVersion {
public:
	
	VersionClassContainer(std::shared_ptr<Dll> dll,
			const std::string& dllFileName,
			const std::string& classNam) :
			dll(dll), dllFileName(dllFileName), className(className) {
		if(dll)
			dll->Get<void (*)(ClassVersion*)>
				((std::string("ClassInitializer_")+className).c_str())(this);
	}
	
	~VersionClassContainer() {
	}
	
	template<typename Class>
	inline void Init() {
		size = sizeof(Class);
		constructor = new Constructor(
				[]()->Object*{return new Class();},
				[](Pointer<Object>ptr)->Object*{return new Class(ptr);}
				);
	}
	
	inline Pointer<Object> New(ConstructorArgs... args) {
		if(constructor) {
			auto ptr = Constructor->New(args...);
			ptr->RTTI(this);
			return ptr;
		}
		return NULL;
	}
	
	inline Pointer<Object> Renew(Pointer<Object> old) {
		if(!(constructor && old))
			return NULL;
		Object* newPtr = constructor->Renew(old);
		newPtr->RTTI(this);
		delete ptr.ReplaceObject(newPtr);
		return old;
	}
	
	inline std::shared_ptr<Dll> GetDll() {
		return dll;
	}
	
	inline size_t GetSize() const {
		return size;
	}
	
	inline const std::string& GetName() const {
		return className;
	}
	
	inline std::set<Pointer<Object>>& GetObjects() {
		return objects;
	}
	
private:
	
	std::set<Pointer<Object>> objects;
	
	std::shared_ptr<Constructor> constructor;
	
	std::shared_ptr<Dll> dll;
	std::string dllFileName;
	std::string className;
	size_t size;
};

#endif

