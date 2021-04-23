
#include "../../include/Class.hpp"
#include "../../include/Pointer.hpp"

#include <cstdio>

class Interface {
public:
	Interface();
	Interface(Pointer<Interface> old);
	virtual ~Interface();
	
	virtual int Func() = 0;
	virtual int V() = 0;
};

