
#include <vector>
#include <iostream>
#include <string>
#include <VTable.hpp>
#include <Invoke.hpp>

class MyClass {
public:
	
	MyClass() {}
	~MyClass() {}
	
	int Make1(std::string& data, float a, int &c) {
		return 342421;
	}
	int Make2(std::string& data, float a) {
		return 65346;
	}
	int Make3(std::string& data, float a) {
		return 543566;
	}
	int Make4(std::string& data, float a) {
		return 6537456;
	}
	int Make5(std::string& data, float a) {
		return 3412453;
	}
	int Make6(std::string& data, float a) {
		return -34214;
	}
	int Make7(std::string& data, float a) {
		return 343254;
	}
	int Make8(std::string& data, float a) {
		return 432143;
	}
	int Make9(std::string& data, float a) {
		return 563545;
	}
	int Make(std::string& data) {
		return -31421;
	}
	int Run(const std::string& mleko) {
		//std::cout << "\n Running my class" << mleko;
		return 789012;
	}
private:
	int something;
	std::string str;
	std::vector<std::string> array;
};

GENERATE_CALLABLE_VTABLE_HEADER(MyClass,
		MEMBER(0, Make)
		MEMBER(1, Run)
		MEMBER(2, Make1)
		MEMBER(3, Make2)
		MEMBER(4, Make3)
		MEMBER(5, Make4)
		MEMBER(6, Make5)
		MEMBER(7, Make6)
		MEMBER(8, Make7)
		MEMBER(9, Make8)
		MEMBER(10, Make9)
		);

