
#pragma once

#include "base.hpp"

class Interface : public Base {
public:
	
	Interface() {}
	virtual ~Interface() override {}
	
	virtual int Getter()=0;
};

