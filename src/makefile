CPP_FILES := $(wildcard *.cpp)
OBJ_FILES := $(addprefix obj/, $(notdir $(CPP_FILES:.cpp=.o)))

CC=g++
CFLAGS=-g -c -Wall

all: $(OBJ_FILES)

obj/%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm obj/*.o
