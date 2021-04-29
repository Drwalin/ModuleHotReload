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

#ifndef VERSION_CLASS_CONTAINER_HPP
#define VERSION_CLASS_CONTAINER_HPP

#include <list>
#include <vector>

#include "ClassVersion.hpp"
#include "ClassSource.hpp"

class VersionClassContainer {
public:
	
	VersionClassContainer(const std::string& sourceFile,
			const std::string& className,
			std::shared_ptr<Compiler> compiler) :
			source(sourceFile, className, compiler) {
		auto initialVersion = source.CompileNewVersion();
		versions.emplace_back(initialVersion);
	}
	
	~VersionClassContainer() {
		std::vector<std::shared_ptr<Dll>> dlls;
		dlls.reserve(versions.size());
		for(auto it : versions)
			dlls.emplace_back(it.GetDll());
		versions.clear();
		dlls.clear();
	}
	
	
	inline bool Recompile() {
		auto snapshot = source.CompileNewVersion();
		if(snapshot == NULL)
			return false;
		if(version.size() > 0)
			if(versions.back() == snapshot)
				return true;
		versions.emplace_back(snapshot);
		return true;
	}
	
	inline size_t UpdateObjects(size_t count) {
		if(versions.size() <= 1)
			return count;
		if(updateToNewest)
			return UpdateToNewest(count);
		return UpdateToImmidiateNext(count);
	}
	
	inline ClassSource& GetSource(); {
		return source;
	}
	
	inline Pointer<Object> New(ConstructorArgs... args) {
		if(vrsions.size())
			return versions.back()->New(args...);
		return NULL;
	}
	
	inline void RequireUpdateToImmidiateNext() {
		updateToNewest = false;
	}
	
private:
	
	size_t UpdateToNewest(size_t count) {
		while(count>0 && versions.size()>1) {
			size_t initialCount = count;
			auto first = versions.begin();
			auto last = versions.end();
			count = Update(*first, *last, count);
			DestroyUnusedClasses();
			if(initialCount == count)
				break;
		}
		return count;
	}
	
	size_t UpdateToImmidiateNext(size_t count) {
		while(count>0 && versions.size()>1) {
			size_t initialCount = count;
			auto first = versions.begin();
			auto second = first.next();
			count = Update(*first, *second, count);
			DestroyUnusedClasses();
			if(initialCount == count)
				break;
		}
		return count;
	}
	
	size_t Update(std::shared_ptr<ClassVersion> prev,
			std::shared_ptr<ClassVersion> next,
			size_t count) {
		auto it_beg = prev->GetObjects().begin();
		auto it = it_beg;
		auto end = prev->GetObjects().end();
		for(; count>0 && it!=end; --count, ++it)
			next->Renew(*it);
		prev->GetObjects().erase(it_beg, it);
		return count;
	}
	
	void DestroyUnusedClasses() {
		while(versions.size()>1 && versions.front()->GetObjects().empty()) {
			auto dll = (*it)->GetDll();				
			versions.erase(versions.begin());
			dll = NULL;
		}
	}
	
private:
	
	bool updateToNewest;
	
	std::list<std::shard_ptr<ClassVersion>> versions;
	ClassSource source;
};

#endif

