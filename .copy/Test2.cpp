// Copyright (C) 2021 Marek Zalewski aka Drwalin
#include "example\Example1_out.cpp"

/*
template<typename T>
class IsPointer {
public:
	inline static const bool value = false;
};

template<typename T>
class IsPointer<T*> {
public:
	inline static const bool value = true;
};

template<typename T>
class IsReference {
public:
	inline static const bool value = false;
};

template<typename T>
class IsReference<T&> {
public:
	inline static const bool value = true;
};

template<typename T>
class IsPrimitive {
public:
	inline static const bool value = false;
};

#define PRIMITIVE_TYPE_SPECIALISATION(TYPE) \
	template<> \
	class IsPrimitive<TYPE> { \
	public: \
		inline static const bool value = true; \
	}
PRIMITIVE_TYPE_SPECIALISATION(char);
PRIMITIVE_TYPE_SPECIALISATION(short);
PRIMITIVE_TYPE_SPECIALISATION(int);
PRIMITIVE_TYPE_SPECIALISATION(long);
PRIMITIVE_TYPE_SPECIALISATION(long long);
PRIMITIVE_TYPE_SPECIALISATION(unsigned char);
PRIMITIVE_TYPE_SPECIALISATION(unsigned short);
PRIMITIVE_TYPE_SPECIALISATION(unsigned int);
PRIMITIVE_TYPE_SPECIALISATION(unsigned long);
PRIMITIVE_TYPE_SPECIALISATION(unsigned long long);
PRIMITIVE_TYPE_SPECIALISATION(bool);
PRIMITIVE_TYPE_SPECIALISATION(float);
PRIMITIVE_TYPE_SPECIALISATION(double);
#undef PRIMITIVE_TYPE_SPECIALISATION





template<typename T>
class ToVoidClass;

template<typename T>
class ToVoidClass<T*> {
public:
	inline static void* Convert(T* arg) {
		return arg;
	}
};

#define PRIMITIVE_TYPE_SPECIALISATION(TYPE) \
	template<> \
	class ToVoidClass<TYPE> { \
	public: \
		inline static void* Convert(TYPE arg) { \
			return *(void**)&arg; \
		} \
	}
PRIMITIVE_TYPE_SPECIALISATION(char);
PRIMITIVE_TYPE_SPECIALISATION(short);
PRIMITIVE_TYPE_SPECIALISATION(int);
PRIMITIVE_TYPE_SPECIALISATION(long);
PRIMITIVE_TYPE_SPECIALISATION(long long);
PRIMITIVE_TYPE_SPECIALISATION(unsigned char);
PRIMITIVE_TYPE_SPECIALISATION(unsigned short);
PRIMITIVE_TYPE_SPECIALISATION(unsigned int);
PRIMITIVE_TYPE_SPECIALISATION(unsigned long);
PRIMITIVE_TYPE_SPECIALISATION(unsigned long long);
PRIMITIVE_TYPE_SPECIALISATION(bool);
PRIMITIVE_TYPE_SPECIALISATION(float);
PRIMITIVE_TYPE_SPECIALISATION(double);
#undef PRIMITIVE_TYPE_SPECIALISATION

template<typename T>
inline void* ToVoidArgument(T arg) { return ToVoidClass<T>::Convert(arg); }


template<typename T>
class ToVoidClass<T&> {
public:
	inline static void* Convert(T&arg) {
		return (void*)&arg;
	}
};









template<typename T>
class FromVoidArgument {
public:
	T value;
	FromVoidArgument(void* arg) : value(*(T*)arg) {}
};

template<typename T>
class FromVoidArgument<T*> {
public:
	T* value;
	FromVoidArgument(void* arg) : value((T*)arg) {}
};

template<typename T>
class FromVoidArgument<T&> {
public:
	T& value;
	FromVoidArgument(void* arg) : value(*(T*)arg) {}
};

#define PRIMITIVE_TYPE_SPECIALISATION(TYPE) \
	template<> \
	class FromVoidArgument<TYPE> { \
		public: \
		TYPE value; \
		FromVoidArgument(void* ptr) : value(*(TYPE*)&ptr) {}\
	}
PRIMITIVE_TYPE_SPECIALISATION(char);
PRIMITIVE_TYPE_SPECIALISATION(short);
PRIMITIVE_TYPE_SPECIALISATION(int);
PRIMITIVE_TYPE_SPECIALISATION(long);
PRIMITIVE_TYPE_SPECIALISATION(long long);
PRIMITIVE_TYPE_SPECIALISATION(unsigned char);
PRIMITIVE_TYPE_SPECIALISATION(unsigned short);
PRIMITIVE_TYPE_SPECIALISATION(unsigned int);
PRIMITIVE_TYPE_SPECIALISATION(unsigned long);
PRIMITIVE_TYPE_SPECIALISATION(unsigned long long);
PRIMITIVE_TYPE_SPECIALISATION(bool);
PRIMITIVE_TYPE_SPECIALISATION(float);
PRIMITIVE_TYPE_SPECIALISATION(double);
#undef PRIMITIVE_TYPE_SPECIALISATION

template<typename T>
class FromVoidArgument<const T&> {
public:
	const T& value;
	FromVoidArgument(void* arg) : value(*(T*)arg) {}
};






template<typename T, typename... FuncArgs>
class Caller {
public:
	
	template<typename... PassedArgs>
	class Internal {
	public:
		template<typename Last>
		inline static T Call(
				T (*f)(FuncArgs...),
				void** args,
				PassedArgs... passedArgs) {
			FromVoidArgument<Last> value(args[0]);
			return f(passedArgs..., value.value);
		}
		
		template<typename First, typename Second, typename... NextArgs>
		inline static T Call(
				T (*f)(FuncArgs...),
				void** args,
				PassedArgs... passedArgs) {
			FromVoidArgument<First> value(args[0]);
			return
				Internal<PassedArgs..., First>::template
				Call<Second, NextArgs...>(
						f,
						args+1,
						passedArgs...,
						value.value);
		}
		
	};
	
	class InternalEmpty {
	public:
		
		inline static T Call(
				T (*f)(),
				void** args) {
			return f();
		}
			
		template<typename First, typename... NextArgs>
		inline static T Call(
				T (*f)(FuncArgs...),
				void** args) {
			FromVoidArgument<First> value(args[0]);
			return
				Internal<First>::template
				Call<NextArgs...>(
						f,
						args+1,
						value.value);
		}
	};
};






#define INVALID_RETURN_TYPE "Cannot invoke function with not primitive type or pointer or reference"
#define RETURN_TYPE_ASSERT \
	static_assert(sizeof(T) <= 8, INVALID_RETURN_TYPE); \
	static_assert(IsPointer<T>::value || IsPrimitive<T>::value || IsReference<T>::value, INVALID_RETURN_TYPE);
//	static_assert(IsReference<T>::value, "Is not reference" INVALID_RETURN_TYPE); \
//	static_assert(IsPointer<T>::value, "Is not pointer" INVALID_RETURN_TYPE); \
//	static_assert(IsPrimitive<T>::value, "Is not primitive" INVALID_RETURN_TYPE); \

template<typename T, typename... Args>
void* Call(T (*f)(Args...), void** args) {
	RETURN_TYPE_ASSERT;
	T value = Caller<T, Args...>::
			InternalEmpty::
			template Call<Args...>(f, args);
	void* ret = *(void**)&value;
	return ret;
}










template<typename... Args>
inline void PrepareCallArgs(void** array, Args... args);

template<>
inline void PrepareCallArgs(void** array) {}

template<typename Last>
inline void PrepareCallArgs(void** array, Last last) {
	array[0] = ToVoidArgument(last);
}

template<typename First, typename Second, typename... Args>
inline void PrepareCallArgs(void** array, First first, Second second, Args... args) {
	array[0] = ToVoidArgument(first);
	PrepareCallArgs(array+1, second, args...);
}

#warning Invoke: when giving refrence to an object it is needed to pass pointer to object
template<typename T, typename... Args>
inline T Invoke(void* (*Function)(void**), Args... args) {
	RETURN_TYPE_ASSERT;
	void* vargs[sizeof...(Args)];
	PrepareCallArgs(vargs, args...);
	void* var = Function(vargs);
	return *(T*)&var;
}

*/









#include "src\Invoke.hpp"

#include <cstdio>
#include <string>

int function(int &a, std::string &b, long* c) {
	int s = a + *c;
	printf("\n received:\n   %i\n   %s\n  %llu", a, b.c_str(), *c);
	a = 987612354;
	b = "Rozlane!!!!";
	return s;
}

void* FunctionCall(void** args) {
	return Call(function, args);
}

int main() {
	int a = 66666;
	std::string b = "Mleko";
	long c = 7689;
	
//	void* args[] = {&a, &b, &c};
	
//	Call(function, args);
	
	
	Invoke<int>(FunctionCall, &a, &b, &c);
	
	printf("\n a later: %i\n b later: %s", a, b.c_str());
	
	
	void* ptr = AllocateObject();
	
	std::string _ = "1234567890qweeertyuiopasdfghjklzxcvbnm:wa";
	Invoke<int>(CallMember, ptr, 1, &_);
	
	FreeObject(ptr);
	
	return 0;
}



