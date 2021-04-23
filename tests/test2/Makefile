
include ../../dep/MakefileSTD/MakefileSTD

CFLAGS = -std=c++17 -Wall -pedantic -I../../dep/DllLoader/include -Ofast -g
SHARED = -shared -fPIC

ifeq ($(platform),win)
else
	LIBS += -ldl
endif

all: main$(EXEC_EXT) ./dll1$(SHARED_EXT) ./dll2$(SHARED_EXT) ./dll3$(SHARED_EXT) ./dll4$(SHARED_EXT)

main$(EXEC_EXT): core$(SHARED_EXT) main.cpp ../../dep/DllLoader/libDllLoader.a
	g++ $^ -o $@ $(CFLAGS) -Wl,-rpath,. $(LIBS)

core$(SHARED_EXT): Interface.cpp ../../dep/DllLoader/libDllLoader.a
	g++ $^ -o $@ $(CFLAGS) $(SHARED) -Wl,-rpath,. $(LIBS)

./%$(SHARED_EXT): ./core$(SHARED_EXT) ./%.cpp
	g++ ./core$(SHARED_EXT) $^ -o $@ $(CFLAGS) $(SHARED) -Wl,-rpath,.,-rpath,..,-rpath,../.. $(LIBS)

../../dep/DllLoader/libDllLoader.a:
	cd "../../dep/DllLoader" && $(MAKE)

.PHONY: clean
clean:
	$(RM) main$(EXEC_EXT) *$(SHARED_EXT)

