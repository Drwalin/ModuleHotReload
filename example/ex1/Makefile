
DEBUGFLAGS = -ggdb3 -g3 -ggdb -g
PROFILEFLAGS = 
FASTFLAGS = -Ofast
INCLUDES = -I../../src
STRIPFLAGS = -s
CXX = g++
SHARED = -shared -fPIC
OBJECT = -c
CXXFLAGS = -m64 -std=c++17 $(INCLUDES)
LIBS =
DLLDIR = dll/tmp

ifeq ($(O), fast)
    CXXFLAGS += $(FASTFLAGS)
endif
ifeq ($(O), debug)
	CXXFLAGS += $(DEBUGFLAGS)
endif
ifeq ($(O), profile)
	CXXFLAGS += $(FASTFLAGS) $(PROFILEFLAGS)
endif

ifdef profile
	CXXFLAGS += $(PROFILEFLAGS)
endif
ifdef strip
	CXXFLAGS += $(STRIPFLAGS)
endif


SYSTEM_TYPE =
ifeq ($(OS), Windows_NT)
    SYSTEM_TYPE = win
endif
ifeq ($(sys), win)
    SYSTEM_TYPE = win
endif
ifeq ($(sys), lin)
    SYSTEM_TYPE = nix
endif
ifeq ($(sys), nix)
    SYSTEM_TYPE = nix
endif
ifeq ($(sys), osx)
    SYSTEM_TYPE = nix
endif


ifeq ($(SYSTEM_TYPE),win)
	LIBS += 
	DLL = .dll
	PRE = 
	EXECUTABLE = main.exe
	RUN = main
else
	LIBS += -ldl 
	DLL = .so
	PRE = ./
	EXECUTABLE = main
	RUN = ./main
	PRE = ./
	RPATH = -Wl,-rpath,
endif

SONAME = -Wl,-soname,




run: dll $(EXECUTABLE)
	$(RUN)
	@echo

$(EXECUTABLE): main.cpp $(PRE)counter$(DLL)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)
	
dll: $(DLLDIR)/example$(DLL)
	
$(DLLDIR)/%$(DLL): %.cpp $(PRE)counter$(DLL)
	$(CXX) -o $@ $< $(CXXFLAGS) $(SHARED) $(RPATH)$(PRE)counter$(DLL) $(LIBS)
	

counter$(DLL): ../../src/InterfaceGenerator.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS) $(SHARED)  $(SONAME)$(PRE)counter$(DLL) $(LIBS)

strip:
	strip $(EXECUTABLE) $(DLLDIR)/*$(DLL) *$(DLL) 


RM = rm -f
.PHONY: clean
clean:
	$(RM) $(EXECUTABLE) $(DLLDIR)/*$(DLL) *$(DLL) gmon.out
