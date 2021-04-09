
ifeq ($(platform),)
	shellstd = posix
	platform = unix
	ifeq ($(OS),Windows_NT)
		platform = win
		shellstd = win
	endif
	ifeq ($(shell uname -a),)
		shellstd = posix
		platform = win
	else ifneq ($(findstring Darwin,$(shell uname -a)),)
		shellstd = posix
		platform = osx
	else ifneq ($(findstring MINGW,$(shell uname -a)),)
		shellstd = posix
		platform = win
	endif
endif

ifeq ($(shellstd),posix)
	RM = rm -f
	MD = mkdir
	CP = cp -r
else
	RM = del /f
	MD = md
	CP = cp
endif

ifneq ($(platform),win)
	SHARED_EXT =.so
	EXEC_EXT =
else
	SHARED_EXT =.dll
	EXEC_EXT =.exe
endif

