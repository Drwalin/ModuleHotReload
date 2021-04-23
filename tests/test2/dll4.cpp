
#include "Interface.hpp"

class Derived : public Interface {
public:
	
	Derived() {
		printf("Derived4()\n");
		v = 4000;
	}
	
	Derived(Pointer<Interface> old) {
		v = old->V();
		printf("Derived4(old) v=%i\n", v);
	}
	
	virtual ~Derived() {
		printf("~Derived4()\n");
	}
	
	virtual int Func() override {
		printf("Func4() : v = %i\n", v);
		return v|99999;
	}
	
	virtual int V() override { return v; }
	
	int v;
};

extern "C" {
	std::shared_ptr<Class<Interface>> GetClass(std::shared_ptr<Dll> dll) {
		return std::shared_ptr<Class<Interface>>(
				Class<Interface>
					::MakeClass<Derived>(dll, "Derived4"));
	}
}

