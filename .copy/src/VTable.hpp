
#ifndef VTABLE_HPP
#define VTABLE_HPP

#include <cinttypes>
#include <memory>
#include <map>
#include <string>
#include <vector>
#include <set>
#include <cstdlib>
#include <atomic>

#include <Invoke.hpp>

typedef void* (*MemberCall)(void**);

struct VTable {
	void* (*allocate)();
	void (*free)(void*);
	MemberCall callMember;
	size_t size;
	std::string name;
	std::atomic<size_t> numberOfObjects;
};

class Object {
public:
	VTable* vtable;
	std::atomic<size_t> references;
	void* self;
	
	Object() {
		references = 0;
		vtable = NULL;
		self = NULL;
	}
	
	inline void AddReference() {
		references++;
	}
	
	inline void RemoveReference() {
		if(vtable && self) {
			references--;
			if(references == 0) {
				vtable->free(self);
				free(this);
			}
		}
	}
	
	void RepaceWith(VTable* vtable) {
		if(this->vtable) {
			vtable->free(self);
			self = NULL;
			this->vtable = NULL;
		}
		if(vtable) {
			this->vtable = vtable;
			self = vtable->allocate();
		}
	}
	
private:
	~Object() {
		if(vtable && self) {
			vtable->free(self);
			self = NULL;
			vtable = NULL;
		}
	}
};

class Pointer {
public:
	
	Pointer() : self(NULL) {
	}
	Pointer(Object* Object) {
		self=Object;
		if(self)
			self->references++;
	}
	Pointer(Pointer& other) {
		self=other.self;
		if(self)
			self->references++;
	}
	Pointer(const Pointer& other) {
		self=(Object*)other.self;
		if(self)
			self->references++;
	}
	Pointer(Pointer&& other) {
		self=other.self;
	}
	~Pointer() {
		if(self) {
			self->RemoveReference();
		}
	}
	
	Pointer& operator=(Pointer& other) {
		self=(Object*)other.self;
		if(self)
			self->references++;
		return *this;
	}
	Pointer& operator=(const Pointer& other) {
		self=(Object*)other.self;
		if(self)
			self->references++;
		return *this;
	}
	Pointer& operator=(Pointer&& other) {
		self=(Object*)other.self;
		return *this;
	}
		
	
	VTable* GetVTable() const {
		if(self) {
			return *(VTable**)self;
		}
	}
	
	template<typename T, typename... Args>
	T Call(size_t methodId, Args... args) {
		if(self && self->vtable)
			return Invoke<T>(self->vtable->callMember, methodId, self->self, args...);
		fprintf(stderr, "\n Trying to call null pointer Object member function");
		fflush(stderr);
	}
	
	Object* self;
};

#define GENERATE_CALLABLE_VTABLE_HEADER(Class, ...) \
	extern "C" { \
	void* AllocateObject(); \
	void FreeObject(void* ptr); \
	void* CallMember(void** args); \
	} \
	VTable vtable = { \
		AllocateObject, \
		FreeObject, \
		CallMember, \
		sizeof(Class), \
		#Class \
	}; \
	VTable* vtable_ptr = &vtable; \
	extern "C" { \
		VTable* GetVTable() { \
			return &vtable; \
		} \
		void* AllocateObject() { \
			return new Class(); \
		} \
		void FreeObject(void* ptr) { \
			if(ptr) \
				delete (Class*)ptr; \
		} \
	}; \
	extern "C" { \
		void* CallMember(void** args_) { \
			Class* self = (Class*)args_[0]; \
			using ThisClass = Class; \
			if(self == NULL) \
				return NULL; \
			void** args = args_+2; \
			switch((unsigned long long)args_[1]) { \
				__VA_ARGS__ \
				default: \
					fprintf(stderr, \
							"\n invalid method %llu id called " \
							"for object %p of type %s", \
							args_[1], \
							self, \
							vtable.name.c_str()); \
					fflush(stderr); \
			} \
			return NULL; \
		} \
	};

#define MEMBER(ID, Method) case ID: return Call(&ThisClass::Method, self, args);

#endif

