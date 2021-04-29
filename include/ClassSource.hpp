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

#ifndef CLASS_SOURCE_HPP
#define CLASS_SOURCE_HPP

#include <DllLoader.h>

#include "Compiler.hpp"
#include "ClassVersion.hpp"

class ClassSourceBase {
public:
	
	ClassSourceBase(const std::string& sourceFile,
			const std::string& className,
			std::shared_ptr<Compiler> compiler) :
		className(className), compiler(compiler) {
		if(source.rfind(".") == std::string::npos)
			sourceFileName = sourceFile;
		else {
			sourceFileDll = std::make_shared<Dll>(sourceFile);
			if(!*sourceFileDll)
				sourceFileDll = NULL;
		}
	}
	
	~ClassSourceBase() {
	}
	
	inline std::shared_ptr<Compiler> GetCompiler() {
		return compiler;
	}
	
protected:
	
	std::string className;
	std::shared_ptr<Compiler> compiler;
	std::string sourceFileName;
	std::shared_ptr<Dll> sourceFileDll;
};

template<typename Base, typename... ConstructorArgs>
class ClassSource : public ClassSourceBase {
public:
	
	using ClassType = ClassVersion<Base, ConstructorArgs...>;
	
	ClassSource(const std::string& sourceFile,
			const std::string& className,
			std::shared_ptr<Compiler> compiler) :
			ClassSourceBase(sourceFile, className, compiler) {
	}
	
	~ClassSource() {
	}
	
	inline std::shared_ptr<ClassType> CompileNewVersion() {
		if(*sourceFileDll)
			return std::shared_ptr<ClassType>(sourceFileDll, className);
		auto dll = compiler->CompileAndLoad(sourceFileName);
		if(dll == NULL)
			return NULL;
		return std::shared_ptr<ClassType>(dll, className);
	}
};

#endif

