
include ../../dependencies/MakefileSTD/MakefileSTD

CFLAGS = -Ofast -std=c++17 -Wall -pedantic -I../../dependencies/DllLoader/include -s
SHARED = -shared -fPIC

ifeq ($(platform),win)
else
	LIBS += -ldl
endif

main$(EXEC_EXT): core$(SHARED_EXT) main.cpp ../../dependencies/DllLoader/libDllLoader.a
	g++ $^ -o $@ $(CFLAGS) -Wl,-rpath,. $(LIBS)
	echo shell = $(SHELL)

core$(SHARED_EXT): base.cpp interface.cpp
	g++ $^ -o $@ $(CFLAGS) $(SHARED) -Wl,-rpath,. $(LIBS)

dll/%$(SHARED_EXT): ./core$(SHARED_EXT) inherited.cpp
	g++ $^ -o $@ $(CFLAGS) $(SHARED) -Wl,-rpath,.,-rpath,..,-rpath,../.. $(LIBS)

../../dependencies/DllLoader/libDllLoader.a:
	cd "../../dependencies/DllLoader" && $(MAKE)

.PHONY: clean
clean:
	$(RM) dll$(S)_*$(SHARED_EXT) main$(EXEC_EXT) *$(SHARED_EXT)

