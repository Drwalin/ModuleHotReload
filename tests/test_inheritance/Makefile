
CFLAGS = -Ofast -std=c++17 -Wall -pedantic -m64 -I../../dependencies/DllLoader/src -s
SHARED = -shared -fPIC

main.exe: core.dll main.cpp
	g++ $^ -o $@ $(CFLAGS)

core.dll: base.cpp interface.cpp
	g++ $^ -o $@ $(CFLAGS) $(SHARED)

dll/%.dll: core.dll inherited.cpp
	g++ $^ -o $@ $(CFLAGS) $(SHARED) -Wl,-rpath,.,-rpath,..,-rpath,../..

.PHONY: clean
clean:
	rm -f *.dll *.exe dll/*.dll

