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

#ifndef COMPILER_HPP
#define COMPILER_HPP

#include <string>
#include <mutex>

#include <DllImporter.h>

class Compiler {
public:
	
	enum {
		PRODUCTION,
		DEBUG,
		DEFAULT
	} Mode;
	
	Compiler(const std::string& compilerInfoFile);
	~Compiler();
	
	std::shared_ptr<Dll> CompileAndLoad(const std::string& file, Mode mode=DEFAULT);
	
private:
	
	std::string Compile(const std::string& file, Mode mode);
	
	std::string compiler;
	
	struct {
		std::string debug;
		std::string production;
		std::string standard;
	} flags;
	
	std::string includes;
	std::string libraries;
	std::string dllLocation;
	std::string tmpLocation;
	
	std::vector<std::string> logs;
	
	std::mutex mutex;
};

#endif

