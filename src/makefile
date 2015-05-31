CPP_FILES := $(wildcard *.cpp)
OBJ_FILES := $(addprefix obj/, $(notdir $(CPP_FILES:.cpp=.o)))

all: $(OBJ_FILES)

obj/%.o: %.cpp
	g++ -c $< -o $@ -Wall

clean:
	rm obj/*.o
