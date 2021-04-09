
include mk/Makefile.mk

CFLAGS = -Ofast -std=c++17 -Wall -pedantic -I../../dependencies/DllLoader/src -s
SHARED = -shared -fPIC

ifeq ($(platform),win)
else
	LIBS += -ldl
endif

main$(EXE_EXT): core$(SHARED_EXT) main.cpp
	g++ $^ -o $@ $(CFLAGS) -Wl,-rpath,. $(LIBS)

core$(SHARED_EXT): base.cpp interface.cpp
	g++ $^ -o $@ $(CFLAGS) $(SHARED) -Wl,-rpath,. $(LIBS)

dll/%$(SHARED_EXT): ./core$(SHARED_EXT) inherited.cpp
	g++ $^ -o $@ $(CFLAGS) $(SHARED) -Wl,-rpath,.,-rpath,..,-rpath,../.. $(LIBS)

.PHONY: clean
clean:
	$(RM) main$(EXE_EXT) *$(SHARED_EXT) dll/*$(SHARED_EXT)

