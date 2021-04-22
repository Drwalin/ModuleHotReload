
#include "../../include/Pointer.hpp"

#include <cstdio>

#define DEBUG {printf("%s %i\n", __PRETTY_FUNCTION__,  __LINE__); fflush(stdout);}

class A {
public:
	A() {
		DEBUG;
	}
	virtual ~A() {
		DEBUG;
	}
	virtual int func() {
		DEBUG;
		return 1;
	}
};

class B : public A {
public:
	B() {
		DEBUG;
	}
	virtual ~B() {
		DEBUG;
	}
	virtual int func() override {
		DEBUG;
		return A::func()+2;
	}
};


int main() {
	DEBUG;
	Pointer<A> a(new A);
	DEBUG;
	a->func();
	DEBUG;
	{
		DEBUG;
		Pointer<A> b = a;
		DEBUG;
		b->func();
		DEBUG;
		Pointer<A> c(new B);
		DEBUG;
		c->func();
		DEBUG;
		a = NULL;
		DEBUG;
	}
	
	
	
	
	
	return 0;
}

