CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix src/obj/, $(notdir $(CPP_FILES:.cpp=.o)))

CC=g++
CFLAGS=-g -Wall

all: lalr1-gen.cpp
	(cd src; make)
	$(CC) $(CFLAGS) lalr1-gen.cpp $(OBJ_FILES) -o lalr1-gen

clean:
	(cd src; make clean)
	rm *.o
	rm gen
