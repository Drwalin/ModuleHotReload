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

#include <Compiler.hpp>

#include <cstdio>
#include <fstream>
#include <cstdlib>

CompilationResult::CompilationResult() {
	returnCode = 0;
}

void CompilationResult::PrintErrors() {
	if(returnCode == 0)
		return;
	std::cout << "\n\n Error " << returnCode << " while compiling: " <<
		fileName << "\n" << log << "\n\n";
}

CompilationResult::operator bool() const {
	return returnCode==0;
}


Compiler::Compiler(const std::string& compilerInfoFile) {
	std::ifstream file(compilerInfoFile);
	if(file.good()) {
		std::getline(file, compiler);
		std::getline(file, debug);
		std::getline(file, production);
		std::getline(file, standard);
		std::getline(file, includes);
		std::getline(file, libraries);
		std::getline(file, dllLocation);
		std::getline(file, tmpLocation);
	}
}

std::shared_ptr<Dll> Compiler::CompileAndLoad(const std::string& file,
		Mode mode) const {
	std::string dllFileName = Compile(file, mode);
	if(dllFileName == "")
		return NULL;
	return std::make_shared<Dll>(dllFileName.c_str());
}

std::string Compiler::Compile(const std::string& file, Mode mode) const {
	std::string dllFileName = dllLocation
		+ GetPathSeparator()
		+ "_"
		+ std::to_string(clock())
		+ "_"
		+ std::to_string(rand())
		+ "_"
		+ std::to_string(tmpname())
		+ GetDllExtension();
	
	std::string logFileName = tmpLocation
		+ GetPathSeparator()
		+ "_"
		+ std::to_string(clock())
		+ "_"
		+ std::to_string(rand())
		+ "_"
		+ std::to_string(tmpname())
		+ ".log";
	
	std::string command = compiler + " "
		+ flags.standard + " "
		+ (mode==DEBUG ? flags.debug : flags.production) + " "
		+ includes + " "
		+ libraries + " "
		+ file + " "
		+ "-o \"" + dllFileName + "\""
		+ " > \"" + logFileName + "\" 2>&1";
	
	int returnCode = system(command.c_str());
	
	CompilationResult result;
	result.fileName = file;
	result.returnCode = returnCode;
	
	std::ifstream log(logFileName, std::ios::binary);
	if(file.good())
		result.log = std::string(
				std::istreambuf_iterator<char>(t),
				std::istreambuf_iterator<char>());
	
	std::guard_lock<std::mutex> lock(mutex);
	logs.Add(result);
	if(returnCode)
		return dllFileName;
	return "";
}

CompilationResult Compiler::PopCompilationResult() {
	return logs.Pop();
}

