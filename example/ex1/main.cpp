
#include <vector>
#include <iostream>
#include <string>
#include <cstdio>

#include <InterfaceGenerator.hpp>
#include <Pointer.hpp>

#include <lib/dll/DllImporter.cpp>

int main() {
	Dll dll("dll/tmp/example");
	if(!dll.IsValid()) {
		std::cout << "\n invalid dll";
		return 1;
	}
	
	VTable* vtable = dll.Get<VTable*(*)()>("MyClass_GetVTable")();
	
	std::cout<<"\n 1";
	Pointer p(new Object());
	std::cout<<"\n 2";
	p.self->self = vtable->allocate();
	std::cout<<"\n 3";
	
	std::cout << "\n vtable:       " << vtable;
	std::cout << "\n self->vtable: " << p.self->self;
	
	std::string str = "Z majna";
	for(int i=2; i<=10; ++i)
		p.Call<int>(i, &str, 13);
	
	
	p.Call<int>(1, &str);
	std::cout<<"\n 4 - str: " << str;
	
	
	return 0;
}

