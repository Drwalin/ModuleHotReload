
#include <interface.hpp>
//#include "../../dependencies/DllLoader/src/DllImporter.cpp"
#include <DllImporter.cpp>

#include <vector>
#include <string>
#include <cstdlib>
#include <conio.h>
#include <ctime>

#include "inherited.cpp"

int main() {
	std::vector<Base*> ptrs;
	std::vector<Dll*>  dlls;
	
	//delete Dll("d").Get<Base*(*)()>("d")();
	
	
	for(int I=0;;) {
		int c = getch();
		
		if(c == 'q')
			break;
		else if(c == 'n') {
			std::string name = std::string("dll/_")+std::to_string(clock());
			printf("\n\n");
			std::string cmd = std::string("make ") + name + ".dll";
			int beg = clock();
			if(system(cmd.c_str()) == 0) {
				Dll* dll = new Dll(name.c_str());
				if(dll && dll->IsValid()) {
					printf("\n loaded in: %.2fs", ((float)(clock()-beg))*0.001f);
					dlls.emplace_back(dll);
					auto alloc = dll->Get<Base*(*)()>("Allocate");
					if(alloc) {
						Base* ptr = alloc();
						ptrs.emplace_back(ptr);
						Inherited* inh = dynamic_cast<Inherited*>(ptr);
						if(inh) {
							printf("\n value %i, v %f", inh->value, inh->v);
						} else {
							printf("\n Cannot cast");
						}
					} else {
						printf("\n Cannot find symbol");
					}
				} else {
					printf("\n Invalid dll");
					printf(" determined in: %.2fs", ((float)(clock()-beg))*0.001f);
				}
			} else {
				printf("\n Compilation failed, please see log.log");
			}
		} else if(c == 'p') {
			for(size_t i=0; i<ptrs.size(); ++i) {
				Interface* in = dynamic_cast<Interface*>(ptrs[i]);
				printf("\n %i", in->Getter());
			}
		} else if(c == 'u') {
			++I;
			printf("\n I = %i", I);
			for(size_t i=0; i<ptrs.size(); ++i) {
				printf("\n %i", ptrs[i]->function(I));
			}
		}
	}
	
	
	delete ptrs[0];
	ptrs[0] = NULL;
	delete dlls[0];
	dlls[0] = NULL;
	
	for(auto& d : ptrs) {
		delete d;
		d = NULL;
	}
	
	for(auto& d : dlls) {
		delete d;
		d = NULL;
	}
	
	return 0;
}

