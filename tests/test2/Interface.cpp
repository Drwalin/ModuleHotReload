
#include "Interface.hpp"

Interface::Interface() {
	printf("Interface()\n");
}

Interface::Interface(Pointer<Interface> old) {
	printf("Interface(old)\n");
}

Interface::~Interface() {
	printf("~Interface()\n");
}

#include "../../include/Class.cpp"
INSTANTIATE_METHODS(Class<Interface>)

