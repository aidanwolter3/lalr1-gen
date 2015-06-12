CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix src/obj/, $(notdir $(CPP_FILES:.cpp=.o)))

all: gen.cpp
	(cd src; make)
	g++ gen.cpp $(OBJ_FILES) -o gen

clean:
	(cd src; make clean)
	rm *.o
	rm gen
