
#include "Interface.hpp"

class Derived : public Interface {
public:
	
	Derived() {
		printf("Derived2()\n");
		v = 20;
	}
	
	Derived(Pointer<Interface> old) {
		v = old->V();
		printf("Derived2(old) v=%i\n", v);
	}
	
	virtual ~Derived() {
		printf("~Derived2()\n");
	}
	
	virtual int Func() override {
		printf("Func2() : v = %i\n", v);
		return v-3;
	}
	
	virtual int V() override { return v; }
	
	int v;
};

extern "C" {
	std::shared_ptr<Class<Interface>> GetClass(std::shared_ptr<Dll> dll) {
		return std::shared_ptr<Class<Interface>>(
				Class<Interface>
					::MakeClass<Derived>(dll, "Derived2"));
	}
}

