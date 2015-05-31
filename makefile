CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix src/obj/, $(notdir $(CPP_FILES:.cpp=.o)))

all: gen.cpp lexer.o
	(cd src; make)
	g++ gen.cpp $(OBJ_FILES) lexer.o -o gen

lexer.o: lexer.cpp
	g++ -c lexer.cpp -o lexer.o

clean:
	(cd src; make clean)
	rm *.o
	rm gen
