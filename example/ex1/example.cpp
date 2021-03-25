
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

