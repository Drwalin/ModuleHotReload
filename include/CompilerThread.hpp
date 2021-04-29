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

#ifndef COMPILER_THREAD_HPP
#define COMPILER_THREAD_HPP

#include "Compiler.hpp"
#include "ClassSource.hpp"
#include "ClassNode.hpp"
#include "ConcurrentQueue.hpp"

#include <thread>
#include <atomic>
#include <mutex>
#include <list>
#include <functional>

class CompilerThread {
public:
	
	CompilerThread(std::shared_ptr<Compiler> compiler) {
		close = false;
		thread = std::thread(std::bind(this, CompilationThread));
		thread.detach();
	}
	
	~CompilerThread() {
		close = true;
		thread.join();
	}
	
	
	inline void RequestUpdate(std::shared_ptr<ClassNodeBase> node) {
		compilationQueue.emplace_back(node);
	}
	
	inline void CompilationThread() {
		while(close == false) {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			if(!compilationQueue.empty()) {
				std::shared_ptr<ClassNodeBase> node;
				if(!compileationQueue.Pop(node))
					continue;
				if(node) {
					std::guard_lock<std::mutex> lock(mutex);
					node->Recompile();
					for(auto it : node->GetChildren())
						RequestUpdate(it);
				}
			}
		}
	}
	
private:
	
	ConcurrentQueue<std::shared_ptr<ClassNodeBase>> compilationQueue;
	
	std::shared_ptr<Compiler> compiler;
	std::mutex mutex;
	std::thread thread;
	std::atomic<bool> close;
};

#endif

