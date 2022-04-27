CXX           = g++
DEL_FILE      = rm
MKDIR         = mkdir
DEL_DIR       = rmdir
FLAGS	      = -c -I./include -fpic -shared -fsanitize=address,leak

all: lib lib/libconst.so bin lib/libfunctions.so lib/libInternalFunctions.so lib/liblexem.so lib/libsemantic.so  bin/main 

lib:
	$(MKDIR) lib

bin:
	$(MKDIR) bin

lib/libconst.so: src/const.cpp
	$(CXX) $? -o $@ $(FLAGS)

lib/libfunctions.so: src/functions.cpp
	$(CXX) $? -o $@ $(FLAGS)

lib/liblexem.so: src/lexem.cpp
	$(CXX) $? -o $@ $(FLAGS)

lib/libInternalFunctions.so: src/InternalFunctions.cpp
	$(CXX) $? -o $@ $(FLAGS)

lib/libsemantic.so: src/semantic.cpp
	$(CXX) $? -o $@ $(FLAGS)

bin/main: src/main.cpp
	$(CXX) $? -I./include -L lib/ -lconst -lfunctions -lInternalFunctions -llexem -lsemantic -o $@ -fsanitize=address,leak

clean:
	$(DEL_FILE) lib/libconst.so lib/libfunctions.so lib/liblexem.so lib/libInternalFunctions.so  lib/libsemantic.so bin/main
	$(DEL_DIR) lib
	$(DEL_DIR) bin
