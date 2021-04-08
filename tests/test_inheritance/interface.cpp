
#include "interface.hpp"

Interface::Interface() : Base() {}

Interface::~Interface() {}

#include <cstdio>

int Interface::FUN() {
	static int d = 0;
	printf("\n d = %d", d);
	return ++d;
}
