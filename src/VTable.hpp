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

#ifndef VTABLE_HPP
#define VTABLE_HPP

#include <cinttypes>
#include <string>
#include <atomic>
#include <mutex>
#include <unordered_set>

struct VTable {
	void** methods;
	void* (*allocate)();
	void (*free)(void*);
	size_t size;
	std::string name;
	uint64_t id;
	uint64_t compilationTimestamp;
	VTable* nextVersion;
	VTable* previousVersion;
	std::unordered_set<void*> objects;
	std::mutex mutex;
};

#endif

