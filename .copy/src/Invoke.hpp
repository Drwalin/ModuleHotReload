// Copyright (C) 2021 Marek Zalewski aka Drwalin

#ifndef INVOKING_FACTORY_HPP
#define INVOKING_FACTORY_HPP

#include <cstddef>

// Type validation
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

template<>
class IsPointer<void> {
public:
	inline static const bool value = false;
};


template<typename T>
class IsVoid {
public:
	inline static const bool value = false;
};

template<>
class IsVoid<void> {
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

template<>
class IsReference<void> {
public:
	inline static const bool value = false;
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



// Static asserts
#define INVALID_RETURN_TYPE "Cannot invoke function with not primitive type or pointer or reference"
#define RETURN_TYPE_ASSERT(TYPE) \
	static_assert(IsPointer<TYPE>::value || IsPrimitive<TYPE>::value, INVALID_RETURN_TYPE); \
	static_assert(!IsVoid<TYPE>::value, "Invokable function must return something, void is not supported as return type");



// Converting argument to void*
template<typename T>
class ToVoidClass;/* {
public:
	inline static void* Convert(T arg) {
		return (void*)&arg;
	}
};*/

template<typename T>
class ToVoidClass<T*> {
public:
	inline static void* Convert(T* arg) {
		return arg;
	}
};

template<>
class ToVoidClass<void> {
public:
	inline static void* Convert(void) {
		return NULL;
	}
};

template<typename T>
class ToVoidClass<T&> {
public:
	inline static void* Convert(T& arg) {
		return (void*)&arg;
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



// Converting argument from void*
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



// Receiving end caller
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
};

template<typename T, typename... Args>
void* Call(T (*f)(Args...), void** args) {
	RETURN_TYPE_ASSERT(T);
	T value = Caller<T, Args...>::
			template Internal<>::
			template Call<Args...>(f, args);
	void* ret = *(void**)&value;
	return ret;
}

template<typename T>
void* Call(T (*f)(), void** args) {
	RETURN_TYPE_ASSERT(T);
	T value = f();
	void* ret = *(void**)&value;
	return ret;
}



// Receiving end caller member
template<class Class, typename T, typename... FuncArgs>
class MemberCaller {
public:
	
	template<typename... PassedArgs>
	class Internal {
	public:
		
		template<typename Last>
		inline static T Call(
				T (Class::*f)(FuncArgs...),
				Class* self,
				void** args,
				PassedArgs... passedArgs) {
			FromVoidArgument<Last> value(args[0]);
			return (self->*f)(passedArgs..., value.value);
		}
		
		template<typename First, typename Second, typename... NextArgs>
		inline static T Call(
				T (Class::*f)(FuncArgs...),
				Class* self,
				void** args,
				PassedArgs... passedArgs) {
			FromVoidArgument<First> value(args[0]);
			return
				Internal<PassedArgs..., First>::template
				Call<Second, NextArgs...>(
						f,
						self,
						args+1,
						passedArgs...,
						value.value);
		}
		
	};
};

template<class Class, typename T, typename... Args>
void* Call(T (Class::*f)(Args...), Class* self, void** args) {
	RETURN_TYPE_ASSERT(T);
	T value = MemberCaller<Class, T, Args...>::
			template Internal<>::
			template Call<Args...>(f, self, args);
	void* ret = *(void**)&value;
	return ret;
}

template<class Class, typename T>
void* Call(T (Class::*f)(), Class* self, void** args) {
	RETURN_TYPE_ASSERT(T);
	T value = (self->*f)();
	void* ret = *(void**)&value;
	return ret;
}



// Invoker
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

#warning Invoke threats refrences as values. When reference is needed pleas pass pointer as argument.
template<typename T, typename... Args>
inline T Invoke(void* (*Function)(void**), Args... args) {
	RETURN_TYPE_ASSERT(T);
	void* vargs[sizeof...(Args)];
	PrepareCallArgs(vargs, args...);
	void* var = Function(vargs);
	return *(T*)&var;
}

#endif

