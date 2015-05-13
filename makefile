all: gen.cpp lexer.o
	g++ gen.cpp lexer.o -o gen

lexer.o: lexer.cpp
	g++ -c lexer.cpp -o lexer.o
