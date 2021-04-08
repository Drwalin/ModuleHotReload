
#pragma once

#include "base.hpp"

class Interface : public Base {
public:
	
	Interface();
	virtual ~Interface() override;
	
	static int FUN();
	
	virtual int Getter()=0;
};

