
CFLAGS = -Ofast -s -std=c++17 -Wall -pedantic -m64
SHARED = -shared -fPIC

main.exe: core.dll main.cpp
	g++ $^ -o $@ $(CFLAGS)

core.dll: base.cpp interface.cpp
	g++ $^ -o $@ $(CFLAGS) $(SHARED)

%.dll: core.dll inherited.cpp
	g++ $^ -o $@ $(CFLAGS) $(SHARED)

.PHONY: clean
clean:
	rm -f *.dll *.exe

