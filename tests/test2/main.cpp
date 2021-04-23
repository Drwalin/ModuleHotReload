
#include "../../include/Class.hpp"
#include <DllImporter.h>
#include "Interface.hpp"

int main() {
	std::shared_ptr<Class<Interface>> a, b, c, d;
	{
		std::shared_ptr<Dll> A = std::make_shared<Dll>("dll1");
		std::shared_ptr<Dll> B = std::make_shared<Dll>("dll2");
		std::shared_ptr<Dll> C = std::make_shared<Dll>("dll3");
		std::shared_ptr<Dll> D = std::make_shared<Dll>("dll4");
		
		a = A->Get<std::shared_ptr<Class<Interface>>(*)(std::shared_ptr<Dll>)>("GetClass")(A);
		b = B->Get<std::shared_ptr<Class<Interface>>(*)(std::shared_ptr<Dll>)>("GetClass")(B);
		c = C->Get<std::shared_ptr<Class<Interface>>(*)(std::shared_ptr<Dll>)>("GetClass")(C);
		d = D->Get<std::shared_ptr<Class<Interface>>(*)(std::shared_ptr<Dll>)>("GetClass")(D);
	}
	
	printf("a = 0x%p\n", (void*)a.get());
	
	Pointer<Interface> _1 = a->GetNew();
	Pointer<Interface> _2 = b->GetNew();
	Pointer<Interface> _3 = c->GetNew();
	Pointer<Interface> _4 = d->GetNew();
	
	printf("\n");
	
	_1->Func();	_2->Func();	_3->Func();	_4->Func();
	a->AddNewVersion(b);
	a = NULL;
	_1->Func();	_2->Func();	_3->Func();	_4->Func();
	b->AddNewVersion(c);
	b = NULL;
	_1->Func();	_2->Func();	_3->Func();	_4->Func();
	c->UpdateObjectsAndClearClasses(1, UpdateToNewest);
	_1->Func();	_2->Func();	_3->Func();	_4->Func();
	c->UpdateObjectsAndClearClasses(1, UpdateToImmediateNext);
	_1->Func();	_2->Func();	_3->Func();	_4->Func();
	c->AddNewVersion(d);
	c = NULL;
	_1->Func();	_2->Func();	_3->Func();	_4->Func();
	d->UpdateObjectsAndClearClasses(1, UpdateToNewest);
	_1->Func();	_2->Func();	_3->Func();	_4->Func();
	d->UpdateObjectsAndClearClasses(1, UpdateToNewest);
	_1->Func();	_2->Func();	_3->Func();	_4->Func();
	d->UpdateObjectsAndClearClasses(1, UpdateToNewest);
	_1->Func();	_2->Func();	_3->Func();	_4->Func();
	d->UpdateObjectsAndClearClasses(1, UpdateToNewest);
	_1->Func();	_2->Func();	_3->Func();	_4->Func();
	d->UpdateObjectsAndClearClasses(1, UpdateToNewest);
	_1->Func();	_2->Func();	_3->Func();	_4->Func();
	d->UpdateObjectsAndClearClasses(1, UpdateToNewest);
	_1->Func();	_2->Func();	_3->Func();	_4->Func();
	
	_1 = NULL;
	_2 = NULL;
	_3 = NULL;
	_4 = NULL;
	{
		auto tempDll = d->GetDll();
		d = NULL;
	}
	
	return 0;
}

