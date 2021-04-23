
#include "Interface.hpp"

class Derived : public Interface {
public:
	
	Derived() {
		printf("Derived3()\n");
		v = 300;
	}
	
	Derived(Pointer<Interface> old) {
		v = old->V();
		printf("Derived3(old) v=%i\n", v);
	}
	
	~Derived() {
		printf("~Derived3()\n");
	}
	
	virtual int Func() override {
		printf("Func3() : v = %i\n", v);
		return v*31;
	}
	
	virtual int V() override { return v; }
	
	int v;
};

extern "C" {
	std::shared_ptr<Class<Interface>> GetClass(std::shared_ptr<Dll> dll) {
		return std::shared_ptr<Class<Interface>>(
				Class<Interface>
					::MakeClass<Derived>(dll, "Derived3"));
	}
}

