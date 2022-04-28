CXX           = g++
DEL_FILE      = rm
MKDIR         = mkdir
DEL_DIR       = rmdir
FLAGS         = -c -I./headers -fpic -shared -fsanitize=address,leak


all: lib bin lib/libinterpreter.so  bin/main 

lib:
        $(MKDIR) lib

bin:
        $(MKDIR) bin

lib/libinterpreter.so: src/interpreter.cpp
        $(CXX) $? -o $@ $(FLAGS)

bin/main: src/main.cpp
        $(CXX) $? -I./headers -L lib/ -linterpreter -o $@ -fsanitize=address,leak

clean:
        $(DEL_FILE) lib/libinterpreter.so bin/main
        $(DEL_DIR) lib
        $(DEL_DIR) bin

