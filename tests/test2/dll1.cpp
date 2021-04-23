
#include "Interface.hpp"

class Derived : public Interface {
public:
	
	Derived() {
		printf("Derived1()\n");
		v = 1;
	}
	
	Derived(Pointer<Interface> old) {
		v = old->V();
		printf("Derived1(old) v=%i\n", v);
	}
	
	virtual ~Derived() {
		printf("~Derived1()\n");
	}
	
	virtual int Func() override {
		printf("Func1() : v = %i\n", v);
		return v+3;
	}
	
	virtual int V() override { return v; }
	
	int v;
};

extern "C" {
	std::shared_ptr<Class<Interface>> GetClass(std::shared_ptr<Dll> dll) {
		return Class<Interface>
				::MakeClass<Derived>(dll, "Derived1");
	}
}

