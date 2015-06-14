CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix src/obj/, $(notdir $(CPP_FILES:.cpp=.o)))

CC=g++
CFLAGS=-g -Wall

all: gen.cpp
	(cd src; make)
	$(CC) $(CFLAGS) gen.cpp $(OBJ_FILES) -o gen

clean:
	(cd src; make clean)
	rm *.o
	rm gen
