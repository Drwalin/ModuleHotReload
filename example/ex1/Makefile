
DEBUGFLAGS = -ggdb3 -g3 -ggdb -g
PROFILEFLAGS = 
FASTFLAGS = -Ofast
INCLUDES = -I../../$(SRC) -I../../dep/JSON/include -I../../dep/DllLoader/include
STRIPFLAGS = -s
CXX = g++
SHARED = -shared -fPIC
OBJECT = -c
CXXFLAGS = -m64 -std=c++17 $(INCLUDES)
LIBS = -L../../dependencies/DllLoader
MODULEDIR = dll/tmp
CORE_DLLS = counter$(DLL)
SRC = src

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
ifdef static
	CXXFLAGS += -static
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
	DLL = .dll
	EXECUTABLE = main.exe
else
	LIBS += -ldl
	DLL = .so
	EXECUTABLE = main.out
endif

MODULE_RPATH = -Wl,-rpath,.,-rpath,..,-rpath,../..
EXECUTABLE_RPATH = -Wl,-rpath,.,-rpath,dll,-rpath,dll/tmp
CORE_RPATH = -Wl,-rpath,.,-rpath,dll,-rpath,..




run: dll $(EXECUTABLE)
	./$(EXECUTABLE)
	@echo

$(EXECUTABLE): $(SRC)/main.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS) $(EXECUTABLE_RPATH) -lDllLoader
	
dll: $(MODULEDIR)/example$(DLL)
	
$(MODULEDIR)/%$(DLL): $(SRC)/%.cpp $(CORE_DLLS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(SHARED) $(MODULE_RPATH) $(LIBS)
	

counter$(DLL): ../../$(SRC)/InterfaceGenerator.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS) $(SHARED) $(LIBS)


strip:
	strip $(EXECUTABLE) $(MODULEDIR)/*$(DLL) *$(DLL) 



RM = rm -f
.PHONY: clean
clean:
	$(RM) $(EXECUTABLE) $(MODULEDIR)/*$(DLL) *$(DLL) gmon.out
