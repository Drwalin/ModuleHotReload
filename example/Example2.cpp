
#include <vector>
#include <iostream>
#include <string>
#include <InterfaceGenerator.hpp>
#include <Pointer.hpp>

class MyClass {
public:
	
	MyClass() {}
	~MyClass() {}
	
	int Make1(std::string* data, float a) {
		std::cout << "\n Running my class" << *data << "  " << __PRETTY_FUNCTION__;
		return 342421;
	}
	int Make2(std::string* data, float a) {
		std::cout << "\n Running my class" << *data << "  " << __PRETTY_FUNCTION__;
		return 65346;
	}
	int Make3(std::string* data, float a) {
		std::cout << "\n Running my class" << *data << "  " << __PRETTY_FUNCTION__;
		return 543566;
	}
	int Make4(std::string* data, float a) {
		std::cout << "\n Running my class" << *data << "  " << __PRETTY_FUNCTION__;
		return 6537456;
	}
	int Make5(std::string* data, float a) {
		std::cout << "\n Running my class" << *data << "  " << __PRETTY_FUNCTION__;
		return 3412453;
	}
	int Make6(std::string* data, float a) {
		std::cout << "\n Running my class" << *data << "  " << __PRETTY_FUNCTION__;
		return -34214;
	}
	int Make7(std::string* data, float a) {
		std::cout << "\n Running my class" << *data << "  " << __PRETTY_FUNCTION__;
		return 343254;
	}
	int Make8(std::string* data, float a) {
		std::cout << "\n Running my class" << *data << "  " << __PRETTY_FUNCTION__;
		return 432143;
	}
	int Make9(std::string* data, float a) {
		std::cout << "\n Running my class" << *data << "  " << __PRETTY_FUNCTION__;
		return 563545;
	}
	int Make(std::string* data) {
		std::cout << "\n Running my class" << *data << "  " << __PRETTY_FUNCTION__;
		return -31421;
	}
	int Run(std::string* mleko) {
		std::cout << "\n Running my class" << *mleko << "  " << __PRETTY_FUNCTION__;
		*mleko = "Zmieniononnnnneee 123";
		return 789012;
	}
private:
	int something;
	std::string str;
	std::vector<std::string> array;
};

GENERATE_CALLABLE_VTABLE_HEADER(MyClass,
		0, _(Make),
		1, _(Run),
		2, _(Make1),
		3, _(Make2),
		4, _(Make3),
		5, _(Make4),
		6, _(Make5),
		7, _(Make6),
		8, _(Make7),
		9, _(Make8),
		10, _(Make9)
		);

#define MAG Ciak

#define APPEND(A, B) REEVALUATE(A, B)
#define REEVALUATE(S, B) S##B
int APPEND(MAG,Ups)() {
	printf("\n elo");
	return 123;
}

#include <InterfaceGenerator.cpp>

#include <cstdio>
int main() {
	std::cout<<"\n 1";
	Pointer p(new Object());
	std::cout<<"\n 2";
	p.self->self = MyClass_AllocateObject();
	std::cout<<"\n 3";
	
	std::cout << "\n vtable:       " << &MyClass_vtable;
	std::cout << "\n self->vtable: " << p.self->self;
	
	std::string str = "Z majna";
	p.Call<int>(1, &str);
	for(int i=2; i<=10; ++i)
		p.Call<int>(i, &str, 13);
	
	
	std::cout<<"\n 4 - str: " << str;
	
	CiakUps();
	
	
	return 0;
}

