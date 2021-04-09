
#include "interface.hpp"

#include <cstdio>
#include <cstdlib>

class Inherited : public Interface {
public:
	
	Inherited() : Interface() {
		printf("\n A");
		value = 1;
	}
	virtual ~Inherited() {
		printf("\n A : %i, %i+1", value, value+1);
	}
	
	virtual int function(int arg) override {
		value += arg+1;
		return value-1-Interface::FUN();
	}
	
	virtual int Getter() override {
//		printf("\n G");
		return value;
	}
	
	int value;
	float v;
	long long d;
	long long g;
	char a;
	long double p;
};

extern "C" Base* Allocate() {
	printf("\n Base = %llu\n Interface = %llu\n Inherited = %llu",
			sizeof(Base), sizeof(Interface), sizeof(Inherited));
	Base* p = new Inherited();
	
	printf("\n");
	
	return p;
}

