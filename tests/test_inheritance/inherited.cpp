
#include "interface.hpp"

#include <cstdio>
#include <cstdlib>

class Inherited : public Interface {
public:
	
	Inherited() : Interface() {
		printf("\n G");
		value = 17;
	}
	virtual ~Inherited() {
		printf("\n G : %i, %i+17", value, value+17);
	}
	
	virtual int function(int arg) override {
		value += arg+17;
		return value-17-Interface::FUN();
	}
	
	virtual int Getter() override {
//		printf("\n G");
		return value;
	}
	
	int value;
	float v;
};

extern "C" Base* Allocate() {
	printf("\n Base = %llu\n Interface = %llu\n Inherited = %llu",
			sizeof(Base), sizeof(Interface), sizeof(Inherited));
	Base* p = new Inherited();
	
	printf("\n");
	
	return p;
}

