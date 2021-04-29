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

template<typename Base, typename... Args>
class ConstructorBase {
public:
	virtual Base* New(Args... args) const = 0;
	virtual Base* Renew(Pointer<Base> old) const = 0;
};

template<typename Final, typename Base, typename... Args>
class Constructor : public ConstructorBase<Base, Args...>{
public:
	virtual Base* New(Args... args) const override {
		return new Final(args...);
	}
	virtual Base* Renew(Pointer<Base> old) const override {
		return new Final(old);
	}
};

template<typename Base, typename... ConstructorArgs>
class ClassVersion {
public:
	
	using ClassType = ClassVersion<Base, ConstructorArgs...>;
	
	VersionClassContainer(std::shared_ptr<Dll> dll,
			const std::string& dllFileName,
			const std::string& classNam) :
			dll(dll), dllFileName(dllFileName), className(className) {
		if(dll)
			dll->Get<void (*)(ClassVersion<Base, ConstructorArgs...>*)>
				((std::string("ClassInitializer_")+className).c_str())(this);
	}
	
	~VersionClassContainer() {
	}
	
	template<typename Class>
	inline void Init() {
		size = sizeof(Class);
		constructor = new Constructor<Class, Base, ConstructorArgs...>();
	}
	
	inline std::shared_ptr<Dll> GetDll() {
		return dll;
	}
	
	inline Pointer<Base> New(ConstructorArgs... args) {
		if(constructor)
			return Constructor->New(args...);
		return NULL;
	}
	
	inline Pointer<Base> Renew(Pointer<Base> old) {
		if(!(constructor && old))
			return NULL;
		Base* newPtr = constructor->Renew(old);
		delete ptr.ReplaceObject(newPtr);
		return old;
	}
	
	inline size_t GetSize() const {
		return size;
	}
	
	inline const std::string& GetName() const {
		return className;
	}
	
	inline std::set<Pointer<Base>>& GetObjects() {
		return objects;
	}
	
private:
	
	std::set<Pointer<Base>> objects;
	
	std::shared_ptr<Constructor<Base, ConstructorArgs...>> constructor;
	
	std::shared_ptr<Dll> dll;
	std::string dllFileName;
	std::string className;
	size_t size;
};

#endif

