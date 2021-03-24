
#include <vector>
#include <string>
#include <cstdio>
#include <string_view>
#include <cstring>
#include <cstdio>

#include "src\TypeName.hpp"

class Klasa {
public:
	int Metoda(std::string, std::string& a, std::vector<int>);
};

int main() {
	printf("\n %s", type_name<decltype(Klasa::Metoda)>());
	printf("\n %s", TypeName<decltype(Klasa::Metoda)>().data());
	
	printf("\n %s", type_name<std::string>());
	printf("\n %s", TypeName<std::string>().data());
	
	return 0;
}


