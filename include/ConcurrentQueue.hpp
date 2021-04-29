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

#ifndef CONCURRENT_QUEUE_HPP
#define CONCURRENT_QUEUE_HPP

#include <list>
#include <mutex>

template<typename T>
class ConcurrentQueue {
public:
	
	ConcurrentQueue() = default;
	~ConcurrentQueue() = default;
	
	template<typename T2>
	inline void AddAll(const T2& other) {
		std::guard_lock<std::mutex> lock(mutex);
		queue.insert(other.begin(), other.end());
	}
	
	inline void Add(const T& other) {
		std::guard_lock<std::mutex> lock(mutex);
		queue.emplace_back(other);
	}
	
	inline std::list<T> PopAll() {
		std::guard_lock<std::mutex> lock(mutex);
		std::list<T> ret;
		std::swap(ret, queue);
		return ret;
	}
	
	inline T Pop() {
		T ret;
		Pop(ret);
		return ret;
	}
	
	inline bool Pop(T& ret) {
		if(queue.empty())
			return false;
		std::guard_lock<std::mutex> lock(mutex);
		if(queue.empty())
			return false;
		ret = queue.front();
		queue.erase(queue.begin());
	}
	
	inline void Clear() {
		PopAll();
	}
	
private:
	
	std::list<T> queue;
	std::mutex mutex;
};

#endif

